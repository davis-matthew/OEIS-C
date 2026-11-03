#!/usr/bin/env python3
"""
instantiate.py -- make a one-width version of a generic database function.

Database functions are written once, against `seq_t` (values) and `idx_t`
(index). This makes a complete source file for one choice of those types: the
shared helpers the function includes from db/utils are copied in, seq_t and
idx_t are defined at the top, and `a` is renamed after the sequence and width,
e.g. A000045_u32. The fixed widths give C; `big` gives C++ over
db/runtime/oeis_big.hpp.

With --inline, the helpers' bodies -- the shared ones and the file's own --
are also substituted into their callers (see inline_helpers), so what is left
is as close to a single function as can be done safely. The result uses GNU
statement expressions, which GCC and Clang accept in C and C++.

Each output is its own translation unit -- its helpers are static and its
seq_t is local -- so any number of them link into one program.

Examples
--------
    instantiate.py db/A000/A000045.c --width u32 -o A000045_u32.c
    instantiate.py db/A000/A000045.c --width u64 --index u32
    instantiate.py db/A000/A000045.c --width u64 --inline -o A000045_u64.c
    instantiate.py db/A000/A000045.c --width big -o A000045_big.cpp --check
        # then: g++ -std=gnu++17 -I db/runtime ... -lgmpxx -lgmp
"""

import argparse
import re
import subprocess
import sys
import tempfile
from dataclasses import dataclass
from pathlib import Path

RUNTIME_DIR = Path(__file__).resolve().parent.parent / "db" / "runtime"
WIDTHS = (8, 16, 32, 64, 128)


# ── WIDTHS ─────────────────────────────────────────────────────────────────

@dataclass(frozen=True)
class Variant:
    name: str            # u32, i64, big, ...
    ctype: str           # the C type of seq_t ("" for big)
    bits: int            # 0 for big
    signed: bool

    @property
    def big(self):
        return self.bits == 0


def fixed(bits, signed):
    if bits == 128:
        ctype = "__int128" if signed else "unsigned __int128"
    else:
        ctype = f"{'' if signed else 'u'}int{bits}_t"
    return Variant(f"{'i' if signed else 'u'}{bits}", ctype, bits, signed)


BIG = Variant("big", "", 0, True)


def parse_variant(name):
    """'u32', 'i128', 'big' -> Variant."""
    if name == "big":
        return BIG
    if name[:1] in "ui" and name[1:].isdigit() and int(name[1:]) in WIDTHS:
        return fixed(int(name[1:]), name[0] == "i")
    raise ValueError(f"unknown width {name!r}: use u8..u128, i8..i128 or big")


def index_type(offset):
    return "uint64_t" if offset >= 0 else "int64_t"


# ── A MINIMAL C SCANNER ────────────────────────────────────────────────────

def mask(code, directives=True):
    """`code` with comments and the contents of string and character literals
    blanked, and (unless directives is False) each preprocessor directive
    replaced by spaces ending in ';' -- the same length, so positions carry
    over. Renaming uses directives=False, so names inside macros are found."""
    out = list(code)
    n, i = len(code), 0

    def blank(a, b):
        for k in range(a, b):
            if out[k] != "\n":
                out[k] = " "

    while i < n:
        if code.startswith("//", i):
            j = code.find("\n", i)
            j = n if j < 0 else j
            blank(i, j)
            i = j
        elif code.startswith("/*", i):
            j = code.find("*/", i + 2)
            j = n if j < 0 else j + 2
            blank(i, j)
            i = j
        elif code[i] in "\"'":
            quote, j = code[i], i + 1
            while j < n and code[j] != quote and code[j] != "\n":
                j += 2 if code[j] == "\\" else 1
            blank(i + 1, min(j, n))
            i = j + 1
        else:
            i += 1

    if not directives:
        return "".join(out)
    # preprocessor directives, with their continuation lines
    pos, continued = 0, False
    text = "".join(out)
    for line in text.split("\n"):
        if continued or line.lstrip().startswith("#"):
            for k in range(pos, pos + len(line)):
                out[k] = " "
            continued = line.rstrip().endswith("\\")
            if not continued and pos + len(line) < n:
                out[pos + len(line)] = ";"
        pos += len(line) + 1
    return "".join(out)


# ── INSTANTIATION ──────────────────────────────────────────────────────────

INDEX_TYPES = {f"{s}{b}": f"{'u' if s == 'u' else ''}int{b}_t"
               for s in "ui" for b in (8, 16, 32, 64)}
_UTILS_INCLUDE = re.compile(r'^\s*#\s*include\s*"\.\./utils/(\w+)\.h"\s*$', re.M)
_DEP_INCLUDE = re.compile(r'^\s*#\s*include\s*"(\w+)\.h"\s*$', re.M)
_GUARD = re.compile(r"^[ \t]*#[ \t]*(?:ifndef|define)[ \t]+OEISC?_UTILS_\w+_H\b.*\n?"
                    r"|^[ \t]*#[ \t]*endif\b.*\n?(?=\s*\Z)", re.M)


def helper_chain(names, utils_dir):
    """The helper headers to inline, each once, dependencies before dependents."""
    order, seen = [], set()

    def visit(name):
        if name in seen:
            return
        seen.add(name)
        path = utils_dir / f"{name}.h"
        if not path.is_file():
            sys.exit(f"instantiate: helper {name}.h not found in {utils_dir}")
        text = path.read_text()
        for dep in _DEP_INCLUDE.findall(text):
            visit(dep)
        order.append((name, _DEP_INCLUDE.sub("", text).strip()))

    for n in names:
        visit(n)
    return order


def rename_a(code, new_name):
    """Every call or definition of `a` outside comments and strings (macros included)."""
    m = mask(code, directives=False)
    out, last = [], 0
    for hit in re.finditer(r"(?<![\w.>])a(?=\s*\()", m):
        out.append(code[last:hit.start()])
        out.append(new_name)
        last = hit.end()
    out.append(code[last:])
    return "".join(out)


# ── --inline: HELPER BODIES INTO THEIR CALLERS ─────────────────────────────
#
# A call f(x, y) to a helper `static R f(T1 p, T2 q) { ... }` becomes a GNU
# statement expression, which GCC and Clang accept in C and C++ alike:
#
#     ({ T1 f_1_a0 = (x); T2 f_1_a1 = (y); R f_1_r;
#        { T1 p = f_1_a0; T2 q = f_1_a1; ...body, each `return e;` turned into
#          `{ f_1_r = (e); goto f_1_end; }`... }
#        f_1_end: ; f_1_r; })
#
# The arguments are evaluated once, into fresh names, before the helper's own
# names come into scope, so they cannot be captured; the label sits outside the
# body's block, so no jump crosses an initialisation (which C++ forbids).
# Helpers are inlined callees first. One stays a function when it cannot be
# done safely: recursion, its address taken (e.g. a qsort comparator), static
# locals, labels of its own, variadic or unusual parameters, or a name it uses
# that the caller declares for itself.

_IDENT = re.compile(r"[A-Za-z_]\w*")
_C_WORDS = set("""auto break case char const continue default do double else enum extern float
for goto if inline int long register restrict return short signed sizeof static struct switch
typedef union unsigned void volatile while bool _Bool __int128 __inline __inline__""".split())


def _match(m, i):
    """Index just past the bracket matching m[i]."""
    pair = {"{": "}", "(": ")", "[": "]"}[m[i]]
    depth = 0
    for j in range(i, len(m)):
        if m[j] == m[i]:
            depth += 1
        elif m[j] == pair:
            depth -= 1
            if depth == 0:
                return j + 1
    return len(m)


def _functions(code):
    """[(name, start, body_start, end)] for each top-level function definition."""
    m = mask(code)
    out, i, start = [], 0, None
    while i < len(m):
        c = m[i]
        if start is None and not (c.isspace() or c == ";"):
            start = i
        if c == "{":
            head = m[start:i].rstrip()
            j = _match(m, i)
            if head.endswith(")"):
                k, depth = len(head) - 1, 0
                while k >= 0:
                    depth += {")": 1, "(": -1}.get(head[k], 0)
                    if depth == 0:
                        break
                    k -= 1
                mm = re.search(r"([A-Za-z_]\w*)\s*$", head[:k])
                if mm and mm.group(1) not in _C_WORDS and "=" not in head[:k]:
                    out.append((mm.group(1), start, i, j))
                    start, i = None, j
                    continue
            i = j
            continue
        if c == ";":
            start = None
        i += 1
    return out


def _split_args(text):
    """Top-level comma split of an argument or parameter list."""
    m = mask(text)
    parts, depth, last = [], 0, 0
    for i, c in enumerate(m):
        if c in "([{":
            depth += 1
        elif c in ")]}":
            depth -= 1
        elif c == "," and depth == 0:
            parts.append(text[last:i])
            last = i + 1
    parts.append(text[last:])
    return [p.strip() for p in parts if p.strip()]


def _signature(code, name, start, body_start):
    """(return type, [(declarator template, param name)]) or None if it is not
    simple. The template has {} where the name goes, so an array parameter
    keeps its decayed type: `seq_t m[2][3]` gives `seq_t (*{})[3]`."""
    sig = " ".join(mask(code[start:body_start]).split())
    at = re.search(rf"\b{name}\s*\(", sig)
    if not at:
        return None
    ret = re.sub(r"\b(static|inline|__inline__|__inline|extern)\b", " ", sig[:at.start()])
    ret = " ".join(ret.replace("const", " ").split()) if "*" not in ret else " ".join(ret.split())
    inner = sig[at.end():sig.rfind(")")].strip()
    if not ret or "(" in ret or "..." in inner:
        return None
    params = []
    for p in ([] if inner in ("", "void") else _split_args(inner)):
        if "(" in p:
            return None                      # a function pointer
        dims = re.findall(r"\[[^\]]*\]", p)
        p = re.sub(r"\[[^\]]*\]", "", p).strip()
        mm = re.match(r"(.*?)([A-Za-z_]\w*)$", p)
        if not mm or not mm.group(1).strip() or mm.group(2) in _C_WORDS:
            return None
        base = mm.group(1).strip().replace("{", "{{").replace("}", "}}")
        if not dims:
            tmpl = base + " {}"
        elif len(dims) == 1:
            tmpl = base + " *{}"                 # the array decays to a pointer
        else:
            tmpl = base + " (*{})" + "".join(dims[1:])
        params.append((tmpl, mm.group(2)))
    return ret, params


def _uses(text, name):
    """Positions of `name` as an identifier outside comments/strings, and
    whether every one of them is a call."""
    m = mask(text)
    pos = [h.start() for h in re.finditer(rf"(?<![\w.>]){name}\b", m)
           if not (h.start() >= 2 and m[h.start() - 2:h.start()] == "->")]
    calls = all(re.match(r"\s*\(", m[p + len(name):]) for p in pos)
    return pos, calls


def _expand(text, name, ret, params, body, counter, cxx=False):
    """Every call of `name` in text replaced by its statement expression (C) or
    an immediately invoked lambda (C++)."""
    m = mask(text)
    out, last = [], 0
    for h in re.finditer(rf"(?<![\w.>]){name}\s*\(", m):
        if h.start() < last:
            continue
        open_at = h.end() - 1
        close = _match(m, open_at)
        args = _split_args(text[open_at + 1:close - 1])
        if len(args) != len(params):
            return None
        if cxx:
            # a lambda with no captures has exactly a call's semantics: the
            # arguments bind to the parameters, and return returns. (GCC
            # miscompiles statement expressions holding class-type values such
            # as oeis_big at -O2, so C++ does not use them.)
            decl = ", ".join(t.format(p) for t, p in params)
            out.append(text[last:h.start()])
            out.append(f"([]({decl}) -> {ret} {body.strip()})({', '.join(args)})")
            last = close
            continue
        counter[0] += 1
        k = f"_inl_{counter[0]}"
        temps = "".join(f"{t.format(f'{k}_a{i}')} = ({x}); " for i, ((t, _), x) in enumerate(zip(params, args)))
        binds = "".join(f"{t.format(p)} = {k}_a{i}; " for i, (t, p) in enumerate(params))
        void = ret == "void"
        inner = _returns(body, f"{k}_r", f"{k}_end", void)
        if inner is None:
            return None
        res = "" if void else f"{ret} {k}_r; "
        tail = "(void)0; " if void else f"{k}_r; "
        out.append(text[last:h.start()])
        # __label__ makes the label local to this block, so a body inlined twice
        # into one function does not define it twice
        out.append(f"({{ __label__ {k}_end; {temps}{res}{{ {binds}{inner} }} {k}_end: ; {tail}}})")
        last = close
    out.append(text[last:])
    return "".join(out)


def _returns(body, var, label, void):
    """The body (without its braces) with each return made an assignment and a jump."""
    inner = body.strip()[1:-1]
    m = mask(inner)
    out, last = [], 0
    for h in re.finditer(r"\breturn\b", m):
        if h.start() < last:
            continue
        j, depth = h.end(), 0
        while j < len(m):
            c = m[j]
            if c in "([{":
                depth += 1
            elif c in ")]}":
                depth -= 1
            elif c == ";" and depth == 0:
                break
            j += 1
        expr = inner[h.end():j].strip()
        out.append(inner[last:h.start()])
        if expr:
            if void:
                out.append(f"{{ (void)({expr}); goto {label}; }}")
            else:
                out.append(f"{{ {var} = ({expr}); goto {label}; }}")
        else:
            out.append(f"goto {label};")
        last = j + 1
    out.append(inner[last:])
    return "".join(out)


def inline_helpers(code, keep=("a",), cxx=False):
    """Inline every helper that can be, callees first; definitions that are no
    longer called are dropped. Returns (code, [helpers kept as functions])."""
    counter = [0]
    kept = []
    for _ in range(64):                  # one helper per pass; the list is re-parsed
        funcs = _functions(code)
        names = {f[0] for f in funcs}
        calls = {}
        for name, s, b, e in funcs:
            # calls only: a parameter named like a function (often `a`) is not one
            called = set(re.findall(r"([A-Za-z_]\w*)\s*\(", mask(code[b:e], directives=False)))
            calls[name] = called & names
        # leaves first: a helper none of whose callees is still an inlinable helper
        progress = False
        for name, s, b, e in funcs:
            if name in keep or name in kept:
                continue
            reason = _cannot_inline(code, name, s, b, e, calls, names)
            if reason:
                kept.append(name)
                continue
            if any(c in names and c != name and c not in keep and c not in kept for c in calls[name]):
                continue                 # inline its callees first
            sig = _signature(code, name, s, b)
            body = code[b:e]
            rest = code[:s] + code[e:]
            new = _expand(rest, name, sig[0], sig[1], body, counter, cxx)
            # a call inside another call's arguments is copied, not expanded, by
            # one pass: repeat until none is left
            for _ in range(32):
                if new is None or not _uses(new, name)[0]:
                    break
                new = _expand(new, name, sig[0], sig[1], body, counter, cxx)
            if new is None or _uses(new, name)[0]:
                kept.append(name)
                continue
            code = new
            progress = True
            break
        if not progress:
            break
    return code, kept


def _conditional(code, pos):
    """Whether pos lies inside an #if / #ifdef / #ifndef block."""
    depth = 0
    for m in re.finditer(r"^[ \t]*#[ \t]*(if|ifdef|ifndef|endif)\b", code[:pos], re.M):
        depth += -1 if m.group(1) == "endif" else 1
    return depth > 0


def _cannot_inline(code, name, s, b, e, calls, names):
    if name in calls[name]:
        return "recursive"
    funcs = _functions(code)
    if sum(f[0] == name for f in funcs) > 1 or _conditional(code, s):
        return "defined conditionally"       # #ifdef branches: the parser sees them all
    # a prototype or other mention outside every function body
    m = mask(code)
    outside, last = [], 0
    for _, s2, _, e2 in funcs:
        outside.append(m[last:s2])
        last = e2
    outside.append(m[last:])
    if re.search(rf"(?<![\w.>]){name}\b", " ".join(outside)):
        return "declared at file scope"
    sig = _signature(code, name, s, b)
    if sig is None:
        return "signature"
    # a parameter whose type is a typedef of an array cannot be copied into a temporary
    array_types = set(re.findall(r"\btypedef\b[^;{}]*?\b([A-Za-z_]\w*)\s*\[", mask(code)))
    if any(set(_IDENT.findall(t)) & array_types for t, _ in sig[1]):
        return "array-typed parameter"
    body = mask(code[b:e])
    own = re.sub(r"__label__ _inl_\d+_end;|goto _inl_\d+_end;|_inl_\d+_end:", " ", body)
    if re.search(r"\b(static|goto|__label__|va_start|alloca)\b", own) or re.search(r"^\s*\w+\s*:(?!:)", own, re.M):
        return "static locals or labels"
    rest = code[:s] + code[e:]
    _, all_calls = _uses(rest, name)
    if not all_calls:
        return "address taken"
    in_code = len(re.findall(rf"(?<![\w.>]){name}\b", mask(rest)))
    if len(re.findall(rf"(?<![\w.>]){name}\b", mask(rest, directives=False))) != in_code:
        return "used in a macro"             # a macro body is not expanded here
    # mutual recursion: name reachable from its callees
    stack, seen = list(calls[name]), set()
    while stack:
        c = stack.pop()
        if c == name:
            return "recursive"
        if c in seen or c not in calls:
            continue
        seen.add(c)
        stack += calls[c]
    # a free name of the helper that a caller declares for itself would be captured
    params = {p for _, p in sig[1]}
    free = set(_IDENT.findall(body)) - _C_WORDS - params
    funcs = _functions(code)
    m = mask(code)
    outside, last = [], 0
    for _, s2, _, e2 in funcs:
        outside.append(m[last:s2])
        last = e2
    outside.append(m[last:])
    file_names = names | set(_IDENT.findall(" ".join(outside)))    # functions and file-scope objects
    for other, s2, b2, e2 in funcs:
        if other == name or name not in calls.get(other, ()):
            continue
        caller = mask(code[b2:e2])
        for f in free & file_names:
            if re.search(rf"[\w\*\]]\s+\**{f}\s*[=;,\[)]", caller):
                return f"caller declares {f}"
    return None


def header_value(code, key):
    """A `Key: value` field of the file header, joined across wrapped lines."""
    m = re.search(rf"^\s*\*\s*{key}:\s*(.*(?:\n\s*\*\s{{4,}}\S.*)*)", code, re.M)
    return " ".join(l.strip(" *") for l in m.group(1).splitlines()) if m else None


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("Examples")[0],
                                 formatter_class=argparse.RawDescriptionHelpFormatter,
                                 epilog="Examples" + __doc__.split("Examples")[1])
    ap.add_argument("file", type=Path, help="a database function, db/Annn/Annnnnn.c")
    ap.add_argument("--width", required=True,
                    help="u8 u16 u32 u64 u128, i8 ... i128, or big")
    ap.add_argument("--index", choices=sorted(INDEX_TYPES),
                    help="index type (default: u64, or i64 when the offset is negative); "
                         "narrower than 64 bits is not what the database validated")
    ap.add_argument("-o", "--out", type=Path, help="output file (default: stdout)")
    ap.add_argument("--name", help="function name (default: <A-number>_<width>)")
    ap.add_argument("--check", action="store_true", help="compile the result to make sure")
    ap.add_argument("--inline", action="store_true",
                    help="also inline the helpers' bodies into their callers (GNU statement "
                         "expressions); helpers that cannot be inlined safely stay functions")
    args = ap.parse_args(argv)

    try:
        variant = parse_variant(args.width)
    except ValueError as exc:
        ap.error(str(exc))
    code = args.file.read_text()
    anum = args.file.stem
    offset = header_value(code, "Offset")
    offset = int(offset.split()[0]) if offset else 0
    keywords = header_value(code, "Keywords") or ""
    idx = INDEX_TYPES[args.index] if args.index else index_type(offset)
    if args.index and args.index.startswith("u") and offset < 0:
        print(f"instantiate: warning: {anum} has offset {offset}; an unsigned index "
              "cannot reach it", file=sys.stderr)
    if not variant.big and variant.signed == ("nonn" in re.split(r"[,\s]+", keywords)):
        print(f"instantiate: warning: {anum} is {'' if variant.signed else 'not '}"
              f"nonn; the database validated {'unsigned' if variant.signed else 'signed'} "
              f"widths for it", file=sys.stderr)
    name = args.name or f"{anum}_{variant.name}"

    # the header comment stays (it carries the OEIS attribution); the rest is
    # the generic code, with its utils includes replaced by the helpers inline
    head_end = code.index("*/") + 2 if code.lstrip().startswith("/*") else 0
    header, body = code[:head_end], code[head_end:]
    helpers = helper_chain(_UTILS_INCLUDE.findall(body), args.file.resolve().parent.parent / "utils")
    body = _UTILS_INCLUDE.sub("", body).strip()

    seq_decl = "typedef oeis_big seq_t;" if variant.big else f"typedef {variant.ctype} seq_t;"
    parts = [header.rstrip() + "\n" if header else "",
             f"/* {name}: {anum} instantiated with seq_t = "
             f"{'an arbitrary-precision integer (oeis_big)' if variant.big else variant.ctype}"
             f", idx_t = {idx}, by tools/instantiate.py{' --inline' if args.inline else ''}. */\n",
             "#include <stdint.h>",
             '#include "oeis_big.hpp"' if variant.big else "",
             seq_decl, f"typedef {idx} idx_t;", ""]
    code_parts = []
    for hname, htext in helpers:
        code_parts += [f"/* -- helper {hname} (db/utils/{hname}.h) -- */", _GUARD.sub("", htext).strip(), ""]
    code_parts.append(body)
    code = "\n".join(code_parts)
    if args.inline:
        code, kept = inline_helpers(code, cxx=variant.big)
        if kept:
            print(f"instantiate: kept as functions: {', '.join(kept)}", file=sys.stderr)
    parts.append(rename_a(code, name))
    result = re.sub(r"\n{3,}", "\n\n", "\n".join(p for p in parts if p is not None)).strip() + "\n"

    if args.check:
        with tempfile.TemporaryDirectory() as tmp:
            src = Path(tmp) / ("x.cpp" if variant.big else "x.c")
            src.write_text(result)
            cmd = (["g++", "-std=gnu++17", "-I", str(RUNTIME_DIR)] if variant.big
                   else ["gcc", "-std=c11"]) + ["-fsyntax-only", "-Wall", str(src)]
            p = subprocess.run(cmd, capture_output=True, text=True)
            if p.returncode:
                sys.exit(f"instantiate: the result does not compile:\n{p.stderr}")
            print(f"instantiate: {name} compiles", file=sys.stderr)
    if args.out:
        args.out.write_text(result)
    else:
        sys.stdout.write(result)
    return 0


if __name__ == "__main__":
    sys.exit(main())
