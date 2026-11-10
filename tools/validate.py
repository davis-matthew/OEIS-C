#!/usr/bin/env python3
"""
validate.py -- check database functions against the OEIS, and record the results.

Each function is built at the widths asked for -- the fixed widths as C, `big`
as C++ over db/runtime/oeis_big.hpp, with the types instantiate.py uses --
run on a set of indices, and compared with the OEIS terms. The style says
which indices:

    partial    every term listed in the sequence entry
    standard   those, plus a seeded uniform random sample of the b-file beyond
               the listed terms (--samples, default 16)
    full       every term in the entry and in the b-file

Each fixed width is also run at the last index whose term still fits in it,
where an intermediate overflow is most likely to show.

For each width the result is a bound: the largest n through which every known
term fits and every checked term came out right. The bounds are written into
the Range line of the file's header and a Validated line records how they were
checked. Each sequence's result goes into one row of status.tsv, which has a
row for every OEIS sequence, implemented or not, and STATUS.md summarises it.
A function passes when the arbitrary-precision build matches every checked
term and the 64-bit build every checked term that fits; the other widths only
set bounds. Widths not in a run keep the bounds they had.

The OEIS data comes from a clone of oeisdata (bare or not, never checked out);
b-files come from its LFS store at oeis.org and are cached in data/bfiles:

    git clone --bare --depth 1 https://github.com/oeis/oeisdata.git data/oeisdata.git

Examples
--------
    validate.py                                   # every sequence, every width, standard
    validate.py A000045 --style full
    validate.py A000001-A000999 --width 64 --width big
    validate.py --summary                         # only rewrite status.tsv and STATUS.md
    validate.py --fetch-only                      # only download the b-files
"""

import argparse
import csv
import hashlib
import io
import json
import os
import random
import re
import signal
import subprocess
import sys
import tarfile
import tempfile
import threading
import time
import urllib.error
import urllib.request
from concurrent.futures import ThreadPoolExecutor, wait, FIRST_COMPLETED
from dataclasses import dataclass, field
from datetime import date
from pathlib import Path

from instantiate import BIG, RUNTIME_DIR, WIDTHS, fixed, header_value, index_type

try:
    import resource
except ImportError:                  # Windows: no resource limits
    resource = None

ROOT = Path(__file__).resolve().parent.parent
DB = ROOT / "db"
STATUS_TSV = ROOT / "status.tsv"
STATUS_MD = ROOT / "STATUS.md"
DEFAULT_OEISDATA = ROOT / "data" / "oeisdata.git"
DEFAULT_BFILES = ROOT / "data" / "bfiles"
ERRATA_TSV = Path(__file__).resolve().parent / "bfile_errata.tsv"


def load_errata():
    """{A-number: [(lo, hi), ...]} of b-file terms known to be wrong
    (tools/bfile_errata.tsv; the n column is an index or a range lo-hi)."""
    out = {}
    if ERRATA_TSV.exists():
        for line in ERRATA_TSV.read_text().splitlines():
            parts = line.split("\t")
            if len(parts) >= 2 and not line.startswith("#"):
                lo, _, hi = parts[1].partition("-")
                out.setdefault(parts[0], []).append((int(lo), int(hi or lo)))
    return out


ERRATA = load_errata()


def is_erratum(anum, n):
    return any(lo <= n <= hi for lo, hi in ERRATA.get(anum, ()))


def as_ranges(ns):
    """Sorted indices as "a, b..c, d" text."""
    out, ns = [], sorted(ns)
    i = 0
    while i < len(ns):
        j = i
        while j + 1 < len(ns) and ns[j + 1] == ns[j] + 1:
            j += 1
        out.append(str(ns[i]) if i == j else f"{ns[i]}..{ns[j]}")
        i = j + 1
    return ", ".join(out)

# oeis.org sits behind Cloudflare, which turns away urllib's default agent
USER_AGENT = "OEIS-C validate.py"
STYLES = ("partial", "standard", "full")
COLUMNS = ["id", "status", "style", "result", "checked", "max_n", "known_to", "sign",
           "w8", "w16", "w32", "w64", "w128", "big", "note", "date"]
WIDTH_COLS = {8: "w8", 16: "w16", 32: "w32", 64: "w64", 128: "w128", 0: "big"}
ANUM = re.compile(r"^A\d{6}$")


# ── OEISDATA ───────────────────────────────────────────────────────────────

class OEISData:
    """Sequence entries and b-files out of a clone of oeisdata."""

    def __init__(self, path, cache_dir, bfile_db=None):
        path = Path(path)
        self.git_dir = path / ".git" if (path / ".git").is_dir() else path
        if not (self.git_dir / "HEAD").is_file():
            sys.exit(f"validate: no oeisdata clone at {path}; make one with\n"
                     f"    git clone --bare --depth 1 https://github.com/oeis/oeisdata.git {path}")
        self.cache_dir = Path(cache_dir)
        self._db = None
        self._db_lock = threading.Lock()
        if bfile_db:
            import sqlite3
            self._db = sqlite3.connect(f"file:{Path(bfile_db).resolve()}?mode=ro&immutable=1",
                                       uri=True, check_same_thread=False)
        self._pointers = None

    def git(self, *args, **kw):
        return subprocess.run(["git", "--git-dir", str(self.git_dir), *args],
                              capture_output=True, **kw)

    def commit(self):
        return self.git("rev-parse", "--short", "HEAD", text=True).stdout.strip()

    def all_anums(self):
        names = self.git("ls-tree", "-r", "--name-only", "HEAD", "seq", text=True).stdout
        return {Path(p).stem for p in names.split() if ANUM.match(Path(p).stem)}

    def entries(self, anums):
        """Yield the Entry of each requested A-number that oeisdata has: one
        `git show` each for a few, one pass over a `git archive` stream for many."""
        anums = set(anums)
        if len(anums) <= 200:
            for a in sorted(anums):
                p = self.git("show", f"HEAD:seq/{a[:4]}/{a}.seq")
                if p.returncode == 0:
                    e = parse_seq(p.stdout.decode("utf-8", "replace"))
                    if e:
                        yield e
            return
        proc = subprocess.Popen(["git", "--git-dir", str(self.git_dir), "archive", "HEAD", "seq"],
                                stdout=subprocess.PIPE)
        try:
            with tarfile.open(fileobj=proc.stdout, mode="r|") as tar:
                for member in tar:
                    if member.isfile() and Path(member.name).stem in anums:
                        e = parse_seq(tar.extractfile(member).read().decode("utf-8", "replace"))
                        if e:
                            yield e
        finally:
            proc.stdout.close()
            proc.wait()

    # b-files: every file under files/ is a git-LFS pointer (oid and size); the
    # objects come from the LFS store that .lfsconfig names, via the batch API.

    def _cached(self, anum):
        return self.cache_dir / anum[:4] / f"b{anum[1:]}.txt"

    def bfile(self, anum):
        """The b-file's text, or None when there is none (or it could not be fetched)."""
        if self._db is not None:
            import zlib
            with self._db_lock:
                row = self._db.execute("SELECT data FROM bfiles WHERE anum = ?", (anum,)).fetchone()
            if row:
                return zlib.decompress(row[0]).decode("utf-8", "replace")
        f = self._cached(anum)
        return f.read_text(errors="replace") if f.is_file() else None

    def pointers(self, anums):
        """{anum: (oid, size)} for the b-files of these sequences; text that is
        stored directly rather than through LFS is written to the cache."""
        wanted = {f"b{a[1:]}.txt": a for a in anums}
        ls = self.git("ls-tree", "-r", "HEAD", "files", text=True).stdout
        blobs = []
        for line in ls.splitlines():
            meta, path = line.split("\t", 1)
            a = wanted.get(path.rsplit("/", 1)[-1])
            if a:
                blobs.append((a, meta.split()[2]))
        out = {}
        if not blobs:
            return out
        p = self.git("cat-file", "--batch", input="".join(b + "\n" for _, b in blobs).encode()).stdout
        pos = 0
        for a, _ in blobs:
            head_end = p.index(b"\n", pos)
            size = int(p[pos:head_end].split()[2])
            body = p[head_end + 1:head_end + 1 + size].decode("utf-8", "replace")
            pos = head_end + 1 + size + 1
            fields = dict(l.split(" ", 1) for l in body.splitlines() if " " in l)
            oid = fields.get("oid", "").removeprefix("sha256:")
            if oid:
                out[a] = (oid, int(fields.get("size", 0)))
            else:
                self._store(a, body.encode())
        return out

    def lfs_url(self):
        url = None
        p = self.git("show", "HEAD:.lfsconfig", text=True)
        for line in p.stdout.splitlines():
            key, _, value = line.partition("=")
            if key.strip() == "url":
                url = value.strip()
        if url is None:
            origin = self.git("config", "remote.origin.url", text=True).stdout.strip()
            url = origin.removesuffix("/") + "/info/lfs"
        return url.removesuffix("/")

    def _store(self, anum, data):
        f = self._cached(anum)
        f.parent.mkdir(parents=True, exist_ok=True)
        tmp = f.with_suffix(".tmp")
        tmp.write_bytes(data)
        tmp.replace(f)

    def prefetch(self, anums, interval=2.0, threads=8, timeout=60):
        """Fetch the b-files not yet cached, 100 per batch request; oeis.org is a
        volunteer-run server, so at most one batch request every `interval` seconds."""
        todo = [a for a in sorted(anums) if self.bfile(a) is None]
        if not todo:
            return
        ptrs = self.pointers(todo)
        todo = [(a, *ptrs[a]) for a in todo if a in ptrs and self.bfile(a) is None]
        if not todo:
            return
        print(f"fetching {len(todo)} b-files from oeis.org", file=sys.stderr, flush=True)
        url, failed = self.lfs_url(), 0
        with ThreadPoolExecutor(threads) as pool:
            for i in range(0, len(todo), 100):
                t0 = time.time()
                chunk = todo[i:i + 100]
                links = _lfs_batch(url, chunk, timeout)
                jobs = [(a, pool.submit(_download, links[o], o, timeout)) for a, o, _ in chunk if o in links]
                failed += len(chunk) - len(jobs)
                for a, fut in jobs:
                    data = fut.result()
                    if data is None:
                        failed += 1
                    else:
                        self._store(a, data)
                if i and i % 2000 == 0:
                    print(f"  {i}/{len(todo)} b-files", file=sys.stderr, flush=True)
                time.sleep(max(0.0, interval - (time.time() - t0)))
        if failed:
            print(f"  {failed} b-file(s) could not be fetched; those sequences are checked "
                  "without one", file=sys.stderr)


def _lfs_batch(lfs_url, objs, timeout):
    """Download links for up to 100 objects; waits out 429s and server errors."""
    body = json.dumps({"operation": "download", "transfers": ["basic"],
                       "objects": [{"oid": o, "size": s} for _, o, s in objs]}).encode()
    headers = {"Accept": "application/vnd.git-lfs+json",
               "Content-Type": "application/vnd.git-lfs+json", "User-Agent": USER_AGENT}
    pause = 30
    for _ in range(6):
        try:
            req = urllib.request.Request(lfs_url + "/objects/batch", body, headers)
            with urllib.request.urlopen(req, timeout=timeout) as r:
                ans = json.load(r)
            return {o["oid"]: o["actions"]["download"]["href"]
                    for o in ans.get("objects", []) if "actions" in o}
        except urllib.error.HTTPError as exc:
            if exc.code != 429 and exc.code < 500:
                return {}
        except (urllib.error.URLError, OSError, ValueError):
            pass
        time.sleep(pause)
        pause = min(pause * 2, 600)
    return {}


def _download(href, oid, timeout):
    for attempt in range(4):
        try:
            req = urllib.request.Request(href, headers={"User-Agent": USER_AGENT})
            with urllib.request.urlopen(req, timeout=timeout) as r:
                data = r.read()
            if hashlib.sha256(data).hexdigest() == oid:
                return data
        except (urllib.error.URLError, OSError):
            pass
        time.sleep(5 * (attempt + 1))
    return None


# ── ENTRIES AND B-FILES ────────────────────────────────────────────────────

_FIELD = re.compile(r"^%(\w)\s+(A\d{6})\s?(.*)$")


@dataclass
class Entry:
    anum: str
    offset: int = 0
    terms: list = field(default_factory=list)
    keywords: list = field(default_factory=list)

    @property
    def pairs(self):
        return list(zip(range(self.offset, self.offset + len(self.terms)), self.terms))


def parse_seq(text):
    """One .seq file (`%X Annnnnn payload` lines) -> Entry, or None without terms.
    %O's first component is the index of the first listed term."""
    e = None
    for line in text.splitlines():
        m = _FIELD.match(line)
        if not m:
            continue
        tag, anum, body = m.group(1), m.group(2), m.group(3).strip()
        e = e or Entry(anum)
        if tag in "STU":
            for tok in body.rstrip(",").split(","):
                tok = tok.strip()
                if not tok:
                    continue
                try:
                    e.terms.append(int(tok))
                except ValueError:
                    break        # a malformed term would shift every index after it
        elif tag == "O":
            try:
                e.offset = int(body.split(",")[0])
            except ValueError:
                pass
        elif tag == "K":
            e.keywords += [k for k in body.rstrip(",").split(",") if k]
    return e if e and e.terms else None


def parse_bfile(text):
    """b-file -> sorted [(n, value)]. Lines are `n value`, with # comments."""
    rows = []
    for line in text.splitlines():
        parts = line.split("#", 1)[0].split()
        if len(parts) >= 2:
            try:
                rows.append((int(parts[0]), int(parts[1])))
            except ValueError:
                continue
    return sorted(dict(rows).items())


# ── WIDTHS AND BOUNDS ──────────────────────────────────────────────────────

def fits(variant, v):
    if variant.big:
        return True
    if variant.signed:
        return -(1 << (variant.bits - 1)) <= v < (1 << (variant.bits - 1))
    return 0 <= v < (1 << variant.bits)


def fits_through(variant, known):
    """The largest n such that every known term from the first through n fits
    in the variant (known: sorted (n, a(n))); None if even the first does not."""
    last = None
    for n, v in known:
        if not fits(variant, v):
            break
        last = n
    return last


def exact_through(variant, known, values):
    """(n, first_wrong): n is the largest index through which every term fits and
    every checked one came out right, claimed no further than the variant was
    actually run; first_wrong is the first checked index within the fitting
    range where it was wrong (an intermediate overflow), or None."""
    limit = fits_through(variant, known)
    if limit is None:
        return None, None
    for n, v in known:
        if n > limit:
            break
        if n in values and values[n] != v:
            return (n - 1 if n - 1 >= known[0][0] else None), n
    ran = [n for n in values if n <= limit]
    return (max(ran) if ran else None), None


def parse_width(text):
    """'64', 'u64', 'i64' -> 64; 'big' -> 0. Signedness comes from each sequence."""
    t = text.lower()
    if t == "big":
        return 0
    t = t[1:] if t[:1] in "ui" else t
    if t.isdigit() and int(t) in WIDTHS:
        return int(t)
    raise argparse.ArgumentTypeError(f"unknown width {text!r}: use 8, 16, 32, 64, 128 or big")


# ── BUILD AND RUN ──────────────────────────────────────────────────────────

def prelude(variant, idx_ctype):
    if variant.big:
        return ('#include <stdint.h>\n#include "oeis_big.hpp"\n'
                f"typedef oeis_big seq_t;\ntypedef {idx_ctype} idx_t;\nseq_t a(idx_t n);\n")
    return (f"#include <stdint.h>\ntypedef {variant.ctype} seq_t;\ntypedef {idx_ctype} idx_t;\n"
            f"#define OEIS_SEQ_SIGNED {int(variant.signed)}\n"
            # declared ahead of the code, so a function defined with another
            # signature is a compile error rather than a silently wrong call
            "seq_t a(idx_t n);\n")


# Reads indices on stdin and prints `n value` per line, flushing each one so a
# crash or a timeout still shows how far it got. Values go through 128-bit
# arithmetic, which holds every fixed width.
DRIVER_C = r"""
#include <stdio.h>
#include <stdlib.h>
static void put(seq_t v)
{
#if OEIS_SEQ_SIGNED
    __int128 s = v;
    unsigned __int128 u = s < 0 ? -(unsigned __int128)s : (unsigned __int128)s;
    if (s < 0) putchar('-');
#else
    unsigned __int128 u = v;
#endif
    char buf[48];
    int k = 0;
    do { buf[k++] = (char)('0' + (int)(u % 10)); u /= 10; } while (u);
    while (k) putchar(buf[--k]);
}
int main(void)
{
    char line[256];
    while (fgets(line, sizeof line, stdin)) {
        char *end;
        long long n = strtoll(line, &end, 10);
        if (end == line) continue;
        seq_t v = a((idx_t)n);
        printf("%lld ", n);
        put(v);
        putchar('\n');
        fflush(stdout);
    }
    return 0;
}
"""

DRIVER_CXX = r"""
#include <cstdio>
#include <cstdlib>
#include <iostream>
int main()
{
    char line[256];
    while (std::fgets(line, sizeof line, stdin)) {
        char *end;
        long long n = std::strtoll(line, &end, 10);
        if (end == line) continue;
        seq_t v = a((idx_t)n);
        std::cout << n << ' ' << v.str() << std::endl;   // endl flushes
    }
    return 0;
}
"""


class Builder:
    """Compiles and runs one function at one width, in a scratch directory."""

    def __init__(self, cfg, scratch):
        self.cfg = cfg
        self.scratch = Path(scratch)
        self._lock = threading.Lock()

    def _support(self, variant, idx_ctype):
        with self._lock:
            pre = self.scratch / f"prelude_{variant.name}_{idx_ctype}.h"
            drv = self.scratch / ("driver.cpp" if variant.big else "driver.c")
            if not pre.exists():
                pre.write_text(prelude(variant, idx_ctype))
                if variant.big:
                    # a precompiled header of the prelude (GMP's gmpxx.h, mostly)
                    # halves every arbitrary-precision build; GCC falls back to
                    # the plain header if it cannot use it
                    subprocess.run([self.cfg.cxx, "-std=gnu++17", "-O2", "-fwrapv", "-w", "-I",
                                    str(RUNTIME_DIR), "-x", "c++-header", str(pre), "-o", str(pre) + ".gch"],
                                   capture_output=True)
            if not drv.exists():
                drv.write_text(DRIVER_CXX if variant.big else DRIVER_C)
            return pre, drv

    def compile(self, src, variant, idx_ctype, workdir):
        """(exe or None, compiler output). The source is compiled where it is, so
        its `#include "../utils/NAME.h"` resolve against db/utils."""
        pre, drv = self._support(variant, idx_ctype)
        exe = Path(workdir) / (variant.name + (".exe" if os.name == "nt" else ""))
        if variant.big:
            cmd = [self.cfg.cxx, "-std=gnu++17", "-O2", "-fwrapv", "-w", "-I", str(RUNTIME_DIR),
                   "-include", str(pre), "-x", "c++", str(src), str(drv), "-o", str(exe),
                   "-lgmpxx", "-lgmp", "-lm"]
        else:
            cmd = [self.cfg.cc, "-std=c11", "-O2", "-fwrapv", "-w", "-include", str(pre),
                   "-o", str(exe), str(src), str(drv), "-lm"]
        try:
            p = subprocess.run(cmd, capture_output=True, text=True, timeout=self.cfg.compile_timeout)
        except subprocess.TimeoutExpired:
            return None, f"compiler timed out after {self.cfg.compile_timeout:.0f}s"
        return (exe if p.returncode == 0 else None), (p.stdout + p.stderr).strip()

    def _limits(self):
        cfg = self.cfg

        def apply():
            mem = cfg.mem_mb * 1024 * 1024
            resource.setrlimit(resource.RLIMIT_AS, (mem, mem))
            # CPU time past the wall-clock limit: a run whose parent dies (and so
            # never kills it) still stops
            cpu = int(cfg.batch_timeout) + 5
            resource.setrlimit(resource.RLIMIT_CPU, (cpu, cpu))
            resource.setrlimit(resource.RLIMIT_FSIZE, (1 << 24, 1 << 24))
            resource.setrlimit(resource.RLIMIT_CORE, (0, 0))
        return apply if resource else None

    def run(self, exe, indices):
        """({n: value}, problem): the values it printed, and why it stopped early
        (None if it did not). A crash or timeout keeps the values before it."""
        if not indices:
            return {}, None
        wall = min(self.cfg.run_timeout * len(indices), self.cfg.batch_timeout)
        kw = {"start_new_session": True, "preexec_fn": self._limits()} if os.name == "posix" else {}
        proc = subprocess.Popen([str(exe)], stdin=subprocess.PIPE, stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE, text=True, **kw)
        problem = None
        try:
            out, err = proc.communicate("".join(f"{n}\n" for n in indices), timeout=wall)
        except subprocess.TimeoutExpired:
            try:
                os.killpg(os.getpgid(proc.pid), signal.SIGKILL)
            except (AttributeError, OSError):
                proc.kill()
            out, err = proc.communicate()
            problem = ("timeout", f"time limit ({wall:.0f}s)")
        else:
            if proc.returncode != 0:
                rc = proc.returncode
                why = (f"signal {signal.Signals(-rc).name}" if rc < 0 and -rc in signal.valid_signals()
                       else f"exit status {rc}")
                problem = ("runtime_error", why + (f": {err.strip()[:120]}" if err.strip() else ""))
        values = {}
        for line in out.splitlines():
            parts = line.split()
            if len(parts) == 2:
                try:
                    values[int(parts[0])] = int(parts[1])
                except ValueError:
                    pass
        return values, problem


# ── ONE SEQUENCE ───────────────────────────────────────────────────────────

def sample_bfile(entry, rows, cfg):
    """A uniform random sample of b-file indices beyond the listed terms, seeded
    per sequence (the same draw as the database generator's, with its defaults).
    A finite sequence with every term known (fini, full) and at most
    --full-check-max such terms has all of them checked instead."""
    listed = {n for n, _ in entry.pairs}
    pool = [r for r in rows if r[0] not in listed and r[0] <= cfg.sample_max_n]
    if not pool:
        return []
    if {"fini", "full"} <= set(entry.keywords) and len(pool) <= cfg.full_check_max:
        return pool
    rng = random.Random(f"{cfg.seed}:{entry.anum}")
    return sorted(rng.sample(pool, min(cfg.samples, len(pool))))


def validate(path, entry, rows, widths, cfg, builder):
    """Check one function. Returns a dict: per-width bounds, the outcome, and
    what was checked."""
    head = path.read_text(errors="replace")
    keywords = re.split(r"[,\s]+", header_value(head, "Keywords") or "")
    signed = "nonn" not in keywords
    hoff = header_value(head, "Offset")
    notes = []
    if hoff and int(hoff.split()[0]) != entry.offset:
        notes.append(f"header offset {hoff.split()[0]} but the OEIS says {entry.offset}")

    max_n = cfg.max_n if cfg.max_n is not None else float("inf")
    # an erratum row covers the term wherever it is listed: the entry's own
    # data (a typo there) as well as the b-file
    wrong_listed = [n for n, _ in entry.pairs if is_erratum(entry.anum, n)]
    if wrong_listed:
        notes.append(f"entry term at n = {as_ranges(wrong_listed)} left out (known erratum)")
    pairs = [(n, v) for n, v in entry.pairs if not is_erratum(entry.anum, n)]
    listed = [(n, v) for n, v in pairs if n <= max_n]
    known = dict(pairs)
    wrong = [n for n, _ in rows if is_erratum(entry.anum, n)]
    if wrong:
        notes.append(f"b-file term at n = {as_ranges(wrong)} left out (known erratum)")
        rows = [r for r in rows if not is_erratum(entry.anum, r[0])]
    # a b-file that contradicts the entry's own terms is out of date (the
    # entry was corrected after it): the terms are checked, the b-file is not
    bad = next((n for n, v in sorted(rows) if n in known and known[n] != v), None) if rows else None
    if bad is not None:
        notes.append(f"b-file disagrees with the entry at n = {bad} (b-file not used)")
        rows = []
    if cfg.style == "partial":
        base, style = listed, "partial"
    else:
        known.update(rows)
        if cfg.style == "full":
            base, style = [(n, v) for n, v in sorted(known.items()) if n <= max_n], "full"
        else:
            sample = sample_bfile(entry, rows, cfg)
            base = listed + sample
            style = "standard" if sample else "partial"
            if sample and {"fini", "full"} <= set(entry.keywords) and \
                    set(known) <= {n for n, _ in base}:
                style = "full"
    known_d, known = known, sorted(known.items())
    if not rows and cfg.style != "partial" and bad is None:
        notes.append("no b-file")

    variants = [BIG if w == 0 else fixed(w, signed) for w in widths]
    idx_ctype = index_type(entry.offset)
    res = {"style": style, "sign": "i" if signed else "u", "known_to": known[-1][0],
           "bounds": {}, "range": {}, "result": "pass", "notes": notes}
    base_d = dict(base)
    checked_big = None
    broken = {}                   # compiler message -> the widths it stopped
    with tempfile.TemporaryDirectory(prefix="oeisc_", dir=cfg.tmpdir) as wd:
        for v in variants:
            pts = {n: x for n, x in base if fits(v, x)}
            edge = fits_through(v, known)
            if not v.big and edge is not None and edge <= max_n:
                pts[edge] = known_d[edge]
            exe, log = builder.compile(path, v, idx_ctype, wd)
            if exe is None:
                first = next((l for l in log.splitlines() if "error" in l), log[:200])
                first = first.replace(str(path), path.name).strip()[:200]
                broken.setdefault(first, []).append(v.name)
                res["result"] = "compile_error"
                res["bounds"][v.bits] = "none"
                res["range"][v.bits] = f"{v.name} none"
                continue
            values, problem = builder.run(exe, sorted(pts))
            wrong = [(n, pts[n], values[n]) for n in sorted(pts) if n in values and values[n] != pts[n]]
            missing = [n for n in sorted(pts) if n not in values]
            if v.big:
                checked_big = sorted(n for n in pts if n in values)
                exact, _ = exact_through(v, [(n, base_d[n]) for n in sorted(pts)], values)
                ok = not wrong and not missing
                res["bounds"][0] = "any" if ok else ("none" if exact is None else str(exact))
                res["range"][0] = "big any n" if ok else (
                    "big none" if exact is None else f"big n <= {exact}")
            else:
                exact, _ = exact_through(v, known, values)
                res["bounds"][v.bits] = "none" if exact is None else str(exact)
                res["range"][v.bits] = f"{v.name} none" if exact is None else f"{v.name} n <= {exact}"
            # the reference builds decide the outcome; the others only set bounds
            if v.big or v.bits == 64:
                if wrong:
                    n, e, g = wrong[0]
                    notes.append(f"{v.name}: {len(wrong)} wrong, first a({n}) = {g}, expected {e}")
                    if res["result"] == "pass":
                        res["result"] = "mismatch"
                elif missing:
                    kind, why = problem or ("runtime_error", "no output")
                    notes.append(f"{v.name}: stopped before n = {missing[0]} ({why})")
                    if res["result"] == "pass":
                        res["result"] = kind
    for msg, names in broken.items():
        notes.append(f"{', '.join(names)} {'does' if len(names) == 1 else 'do'} not compile: {msg}")
    if not any(v.big or v.bits == 64 for v in variants) and not broken:
        res["result"] = None                          # nothing in this run decides it
    checked = checked_big if checked_big is not None else sorted(base_d)
    res["checked"] = len(checked)
    res["max_n"] = max(checked) if checked else None
    return res


# ── FILE HEADERS ───────────────────────────────────────────────────────────

def _header_field(text, indent=14, width=78, sep=" "):
    """A header value wrapped under its label, between words (or list items)."""
    text = " ".join(text.replace("*/", "*_/").split())
    lines, line, room = [], "", width - indent
    for word in text.split(sep):
        if line and len(line) + len(sep) + len(word) > room:
            lines.append(line + sep.rstrip())
            line = word
        else:
            line = f"{line}{sep}{word}" if line else word
    lines.append(line)
    return ("\n *" + " " * (indent - 2)).join(lines)


def _field_re(key):
    return re.compile(rf"^ \* {key}:[ \t]*.*\n(?: \*[ \t]{{4,}}\S.*\n)*", re.M)


def update_header(path, res, signed, date_s):
    """Rewrite the Range line (widths not checked keep theirs) and the Validated
    line, keeping the file's line endings."""
    with open(path, newline="", errors="replace") as f:
        raw = f.read()
    eol = "\r\n" if "\r\n" in raw else "\n"
    code = raw.replace("\r\n", "\n")
    end = code.find("*/")
    if end < 0:
        return
    head, body = code[:end], code[end:]
    old = {}
    for part in (header_value(head, "Range") or "").split(","):
        part = part.strip()
        if part:
            old[part.split()[0]] = part
    order = [fixed(w, signed).name for w in WIDTHS] + ["big"]
    for bits, text in res["range"].items():
        old[BIG.name if bits == 0 else fixed(bits, signed).name] = text
    range_text = ", ".join(old[k] for k in order if k in old)
    outcome = res["result"] or "bounds only"
    checked = (f"{res['checked']} terms through n = {res['max_n']}" if res["max_n"] is not None
               else "no terms")
    validated = f"{res['style']}, {outcome}, {checked} ({date_s})"

    range_line = f" * Range:     {_header_field(range_text, sep=', ')}\n"
    valid_line = f" * Validated: {_header_field(validated)}\n"
    if res["result"] is None and _field_re("Validated").search(head):
        valid_line = None             # a run without the reference widths leaves the verdict be
    elif _field_re("Validated").search(head):
        head = _field_re("Validated").sub(lambda m: valid_line, head, count=1)
    if _field_re("Range").search(head):
        head = _field_re("Range").sub(lambda m: range_line + ("" if "Validated:" in head else valid_line),
                                      head, count=1)
    else:
        return
    new = (head + body).replace("\n", eol)
    if new != raw:
        tmp = path.with_suffix(".tmp")
        with open(tmp, "w", newline="") as f:
            f.write(new)
        tmp.replace(path)


# ── STATUS FILES ───────────────────────────────────────────────────────────

def db_files():
    """{anum: path} for every function in the database."""
    return {p.stem: p for p in DB.glob("A[0-9][0-9][0-9]/A[0-9][0-9][0-9][0-9][0-9][0-9].c")}


def read_status():
    if not STATUS_TSV.is_file():
        return {}
    with STATUS_TSV.open(newline="") as f:
        return {r["id"]: {c: r.get(c) or "" for c in COLUMNS}
                for r in csv.DictReader(f, delimiter="\t")}


def write_status(rows):
    tmp = STATUS_TSV.with_suffix(".tmp")
    with tmp.open("w", newline="") as f:
        w = csv.DictWriter(f, COLUMNS, delimiter="\t", lineterminator="\n")
        w.writeheader()
        for k in sorted(rows):
            w.writerow(rows[k])
    tmp.replace(STATUS_TSV)


def reconcile(rows, files, all_anums):
    """Every database file is an implemented row; every other OEIS sequence is
    unimplemented, or skipped if a row already says so (with its reason in note)."""
    for a, p in files.items():
        r = rows.setdefault(a, {c: "" for c in COLUMNS} | {"id": a})
        r["status"] = "implemented"
        if not r["sign"] or not any(r[c] for c in WIDTH_COLS.values()):
            head = p.read_text(errors="replace")[:4000]
            kw = re.split(r"[,\s]+", header_value(head, "Keywords") or "")
            r["sign"] = "u" if "nonn" in kw else "i"
            for part in (header_value(head, "Range") or "").split(","):
                m = re.match(r"\s*([ui]\d+|big)\s+(.*)", part)
                if m:
                    name, val = m.groups()
                    col = "big" if name == "big" else f"w{name[1:]}"
                    r[col] = ("any" if "any" in val else "none" if val.strip() == "none"
                              else val.split("<=")[-1].strip())
    for a in all_anums:
        if a not in files:
            r = rows.setdefault(a, {c: "" for c in COLUMNS} | {"id": a})
            if r["status"] != "skipped":
                r.update({c: "" for c in COLUMNS if c not in ("id", "note")})
                r["status"] = "unimplemented"
    for a, r in rows.items():
        if a not in files and r["status"] == "implemented":
            r["status"] = "unimplemented"


def write_summary(rows, commit):
    from collections import Counter
    impl = [r for r in rows.values() if r["status"] == "implemented"]
    by_status = Counter(r["status"] for r in rows.values())
    total = sum(by_status.values()) or 1
    out = io.StringIO()
    p = lambda s="": print(s, file=out)

    p("# OEIS-C status")
    p()
    p(f"Generated by `tools/validate.py` from [status.tsv](status.tsv) on {date.today().isoformat()}"
      + (f", against oeisdata commit `{commit}`." if commit else "."))
    p("status.tsv has one row per OEIS sequence; its columns are described in the README.")
    p()
    p("## Coverage")
    p()
    p("| Status | Sequences | Share |")
    p("|---|---:|---:|")
    for s in ("implemented", "unimplemented", "skipped"):
        p(f"| {s} | {by_status[s]:,} | {100 * by_status[s] / total:.1f}% |")
    p(f"| **total** | **{total:,}** | |")
    p()

    p("## Validation of implemented sequences")
    p()
    results = ["pass", "mismatch", "compile_error", "runtime_error", "timeout"]
    grid = Counter((r["style"] or "not validated", r["result"] or "-") for r in impl)
    extra = sorted({res for _, res in grid} - set(results) - {"-"})
    cols = results + extra
    p("| Style | " + " | ".join(cols) + " | total |")
    p("|---|" + "---:|" * (len(cols) + 1))
    for style in ("full", "standard", "partial", "not validated"):
        row = [grid[(style, c)] for c in cols]
        n = sum(v for (s, _), v in grid.items() if s == style)
        if n:
            p(f"| {style} | " + " | ".join(f"{v:,}" for v in row) + f" | {n:,} |")
    p()
    p("*full*: every term in the entry and b-file; *standard*: the entry's terms and a random "
      "sample of the b-file; *partial*: the entry's terms only.")
    p()

    # a skip note is "<category>: <reason>"; the table counts the categories (the
    # reasons themselves, one per sequence, are in status.tsv)
    skips = sorted((r for r in rows.values() if r["status"] == "skipped"), key=lambda r: r["id"])
    if skips:
        cats, examples = Counter(), {}
        for r in skips:
            cat = (r["note"] or "(no reason given)").split(": ", 1)[0]
            cats[cat] += 1
            examples.setdefault(cat, []).append(r["id"])
        p("## Skipped")
        p()
        p(f"{len(skips):,} sequences that no generation run could implement: the model declined "
          "(said there is no practical way to compute it) or the generation ran out of time. "
          "status.tsv gives each one's reason.")
        p()
        p("| Reason | Sequences | Examples |")
        p("|---|---:|---|")
        for cat, n in cats.most_common():
            ex = ", ".join(f"[{a}](https://oeis.org/{a})" for a in examples[cat][:3])
            p(f"| {cat} | {n:,} | {ex} |")
        p()

    failing = sorted((r for r in impl if r["result"] not in ("", "pass")), key=lambda r: r["id"])
    if failing:
        p("## Failures")
        p()
        p(f"{len(failing):,} implemented sequences do not pass"
          + (" (the first 200 are listed; status.tsv has them all)." if len(failing) > 200 else "."))
        p()
        p("| Sequence | Result | Note |")
        p("|---|---|---|")
        for r in failing[:200]:
            note = r["note"].replace("|", "\\|")
            p(f"| [{r['id']}](https://oeis.org/{r['id']}) | {r['result']} | {note} |")
        p()
    STATUS_MD.write_text(out.getvalue())


# ── DRIVER ─────────────────────────────────────────────────────────────────

def select(specs, files):
    """'all', 'A000045', '45' or 'A000001-A000999' -> the database A-numbers they name."""
    if not specs or "all" in [s.lower() for s in specs]:
        return sorted(files)
    out = set()
    for s in specs:
        lo, _, hi = s.upper().partition("-")
        try:
            lo_n = int(lo.lstrip("A"))
            hi_n = int(hi.lstrip("A")) if hi else lo_n
        except ValueError:
            sys.exit(f"validate: not a sequence or range: {s!r}")
        hit = [a for a in files if lo_n <= int(a[1:]) <= hi_n]
        if not hit:
            print(f"validate: no database file for {s}", file=sys.stderr)
        out.update(hit)
    return sorted(out)


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("Examples")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 epilog="Examples" + __doc__.split("Examples")[1])
    ap.add_argument("seqs", nargs="*", metavar="SEQ",
                    help="A-numbers or ranges (A000001-A000999), or all (the default)")
    ap.add_argument("-w", "--width", action="append", type=parse_width,
                    help="8, 16, 32, 64, 128 or big (repeatable; default: all of them)")
    ap.add_argument("-s", "--style", choices=STYLES, default="standard")
    ap.add_argument("--summary", action="store_true",
                    help="validate nothing; only bring status.tsv and STATUS.md up to date")
    ap.add_argument("--no-write", action="store_true",
                    help="print the results; change no files")
    ap.add_argument("-j", "--jobs", type=int, default=os.cpu_count() or 1)

    data = ap.add_argument_group("OEIS data")
    data.add_argument("--oeisdata", type=Path, default=Path(os.environ.get("OEISDATA", DEFAULT_OEISDATA)),
                      help="clone of oeisdata (default: data/oeisdata.git, or $OEISDATA)")
    data.add_argument("--bfile-cache", type=Path, default=DEFAULT_BFILES,
                      help="where fetched b-files are kept (default: data/bfiles)")
    data.add_argument("--bfile-db", type=Path,
                      help="SQLite file of b-files (the generator's prefetch_bfiles.py), read first")
    data.add_argument("--no-fetch", action="store_true", help="use only b-files already cached")
    data.add_argument("--fetch-only", action="store_true",
                      help="only fetch the b-files of the selected sequences into the cache")
    data.add_argument("--samples", type=int, default=16, help="b-file terms sampled by standard")
    data.add_argument("--seed", default="oeis", help="sampling seed")
    data.add_argument("--sample-max-n", type=int, default=10000,
                      help="largest index standard samples (default 10000)")
    data.add_argument("--full-check-max", type=int, default=1000,
                      help="standard checks every term of a complete finite sequence with at "
                           "most this many beyond the listed ones (default 1000)")
    data.add_argument("--max-n", type=int, help="largest index ever evaluated (default: no limit)")

    bld = ap.add_argument_group("build and run")
    bld.add_argument("--cc", default=os.environ.get("CC", "gcc"))
    bld.add_argument("--cxx", default=os.environ.get("CXX", "g++"))
    bld.add_argument("--compile-timeout", type=float, default=60.0)
    bld.add_argument("--run-timeout", type=float, default=2.0, help="seconds allowed per index")
    bld.add_argument("--batch-timeout", type=float, default=120.0,
                     help="wall-clock cap for one width's run")
    bld.add_argument("--mem-mb", type=int, default=2048, help="address-space cap (POSIX)")
    bld.add_argument("--tmpdir", help="scratch for builds")
    cfg = ap.parse_args(argv)

    widths = sorted(set(cfg.width or (*WIDTHS, 0)), key=lambda w: w or 999)
    files = db_files()
    oeis = OEISData(cfg.oeisdata, cfg.bfile_cache, cfg.bfile_db)
    if cfg.fetch_only:
        oeis.prefetch(select(cfg.seqs, files))
        return 0
    rows = {} if cfg.no_write else read_status()
    if not cfg.no_write:
        reconcile(rows, files, oeis.all_anums())

    todo = [] if cfg.summary else select(cfg.seqs, files)
    if todo and cfg.style != "partial" and not cfg.no_fetch:
        oeis.prefetch(todo)

    today = date.today().isoformat()
    done, failed, last_flush = 0, 0, time.time()

    def one(entry):
        path = files[entry.anum]
        text = oeis.bfile(entry.anum) if cfg.style != "partial" else None
        rows_b = parse_bfile(text) if text else []
        res = validate(path, entry, rows_b, widths, cfg, builder)
        if not cfg.no_write:
            update_header(path, res, res["sign"] == "i", today)
        return entry.anum, res

    def record(anum, res):
        nonlocal done, failed
        done += 1
        if res["result"] not in (None, "pass"):
            failed += 1
        line = (f"{anum} {res['style']:<8} {res['result'] or 'bounds':<13} "
                + " ".join(f"{'big' if b == 0 else b}:{v}" for b, v in sorted(
                    res["bounds"].items(), key=lambda kv: kv[0] or 999)))
        if res["notes"]:
            line += "  # " + "; ".join(res["notes"])
        print(line, flush=True)
        if cfg.no_write:
            return
        r = rows[anum]
        r.update({"status": "implemented", "known_to": str(res["known_to"]), "sign": res["sign"]})
        # like the Validated line, the verdict is only replaced by a run that decides one
        if res["result"] is not None or not r["style"]:
            r.update({"style": res["style"], "result": res["result"] or "",
                      "checked": str(res["checked"]),
                      "max_n": "" if res["max_n"] is None else str(res["max_n"]),
                      "note": "; ".join(res["notes"]), "date": today})
        for b, v in res["bounds"].items():
            r[WIDTH_COLS[b]] = v

    with tempfile.TemporaryDirectory(prefix="oeisc_build_", dir=cfg.tmpdir) as scratch:
        builder = Builder(cfg, scratch)
        seen = set()
        try:
            with ThreadPoolExecutor(max(1, cfg.jobs)) as pool:
                pending = set()
                for entry in oeis.entries(todo):
                    seen.add(entry.anum)
                    pending.add(pool.submit(one, entry))
                    while len(pending) >= 4 * cfg.jobs:
                        finished, pending = wait(pending, return_when=FIRST_COMPLETED)
                        for f in finished:
                            record(*f.result())
                    if not cfg.no_write and time.time() - last_flush > 60:
                        write_status(rows)          # progress survives an interrupted run
                        last_flush = time.time()
                for f in pending:
                    record(*f.result())
            for a in todo:
                if a not in seen:
                    print(f"{a} not in oeisdata; not checked", file=sys.stderr)
                    if not cfg.no_write:
                        rows[a]["note"] = "not in oeisdata"
        finally:
            if not cfg.no_write:
                write_status(rows)
                write_summary(rows, oeis.commit())
    if todo:
        print(f"{done} checked, {failed} not passing", file=sys.stderr)
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
