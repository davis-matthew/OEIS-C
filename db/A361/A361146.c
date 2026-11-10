/*
 * A361146  a(n) is the sibling of n in the infinite binary tree underlying
 *          A361144.
 * https://oeis.org/A361146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Search/simulation. The tree is built bottom-up like a binary
 *            counter: leaves are added one at a time, and two pending nodes
 *            on the same level merge into their parent (their sum). Each new
 *            leaf is the smallest unused integer v for which v and every sum
 *            it creates up the chain are still unused. a(n...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

typedef struct {
    seq_t *a;
    size_t n, cap;
} UsedSet;

static size_t us_lower(const UsedSet *u, seq_t v) {
    size_t lo = 0, hi = u->n;
    while (lo < hi) {
        size_t mid = lo + (hi - lo) / 2;
        if (u->a[mid] < v) lo = mid + 1; else hi = mid;
    }
    return lo;
}

static bool us_has(const UsedSet *u, seq_t v) {
    size_t i = us_lower(u, v);
    return i < u->n && u->a[i] == v;
}

static void us_ins(UsedSet *u, seq_t v) {
    size_t i = us_lower(u, v);
    size_t j;
    if (i < u->n && u->a[i] == v) return;
    if (u->n == u->cap) {
        size_t nc = u->cap ? u->cap * 2 : 1024;
        if (u->a) u->a = mem_resize(seq_t, u->a, nc);
        else u->a = mem_new(seq_t, nc);
        u->cap = nc;
    }
    for (j = u->n; j > i; j--) u->a[j] = u->a[j - 1];
    u->a[i] = v;
    u->n++;
}

seq_t a(idx_t n) {
    UsedSet U;
    seq_t target = (seq_t)n;
    seq_t slot[140];
    bool has[140], valid[140];
    seq_t low = 1;
    seq_t result = 0;
    bool found = false;
    int i;
    U.a = 0; U.n = 0; U.cap = 0;
    for (i = 0; i < 140; i++) { has[i] = false; valid[i] = false; slot[i] = 0; }
    while (!found) {
        seq_t v;
        seq_t s;
        bool cv;
        int l;
        while (us_has(&U, low)) low++;
        v = low;
        for (;; v++) {
            seq_t t;
            bool ok = true;
            int k = 0;
            if (us_has(&U, v)) continue;
            t = v;
            while (has[k]) {
                seq_t nt;
                if (!valid[k]) break;
                nt = slot[k] + t;
                if (nt < t) break; /* overflow: beyond representable range */
                if (us_has(&U, nt)) { ok = false; break; }
                t = nt;
                k++;
            }
            if (ok) break;
        }
        /* commit leaf v */
        us_ins(&U, v);
        s = v;
        cv = true;
        l = 0;
        while (has[l]) {
            seq_t A = slot[l];
            bool av = valid[l];
            bool nv;
            seq_t ns;
            has[l] = false;
            if (av && cv) {
                if (A == target) { result = s; found = true; }
                else if (s == target) { result = A; found = true; }
            }
            nv = av && cv;
            ns = A + s;
            if (nv && ns < A) nv = false;
            if (nv) us_ins(&U, ns);
            s = ns;
            cv = nv;
            l++;
        }
        slot[l] = s;
        valid[l] = cv;
        has[l] = true;
    }
    if (U.a) mem_free(U.a);
    return result;
}
