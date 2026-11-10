/*
 * A304083  Permutation of nonnegative integers: Minimal subset/superset
 *          bitmask transform of A054429.
 * https://oeis.org/A304083
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0; for n>0, a(n) = the unused k (not in a(0..n-1)) with
 *            bitor(k,a(n-1)) = a(n-1) (submask) or = k (supermask) minimizing
 *            A054429(k), submasks taking priority; computed by direct
 *            simulation with memoization
 * Range:     u8 n <= 237, u16 n <= 16383, u32 n <= 16383, u64 n <= 16383,
 *            u128 n <= 16383, big any n
 * Validated: standard, pass, 104 terms through n = 9878 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static uint64_t *tm_terms = 0;
static idx_t tm_len = 0, tm_cap = 0;
static unsigned char *tm_used = 0;
static uint64_t tm_usedcap = 0;

static void tm_ensure_used(uint64_t v)
{
    if (v < tm_usedcap) return;
    uint64_t nc = tm_usedcap ? tm_usedcap : 1024;
    while (nc <= v) nc <<= 1;
    unsigned char *p = (unsigned char *)realloc(tm_used, (size_t)nc);
    memset(p + tm_usedcap, 0, (size_t)(nc - tm_usedcap));
    tm_used = p;
    tm_usedcap = nc;
}

static unsigned tm_bl(uint64_t x)
{
    unsigned L = 0;
    while (x) { L++; x >>= 1; }
    return L;
}

/* A054429 for k >= 1 */
static uint64_t tm_f(uint64_t k)
{
    unsigned L = tm_bl(k);
    return (((uint64_t)3) << (L - 1)) - 1 - k;
}

static uint64_t tm_next(uint64_t a)
{
    uint64_t best = 0;
    int found = 0;
    uint64_t bf = 0;
    /* submasks */
    uint64_t s = a;
    while (s) {
        if (!tm_used[s]) {
            uint64_t f = tm_f(s);
            if (!found || f < bf) { found = 1; bf = f; best = s; }
        }
        s = (s - 1) & a;
    }
    if (found) return best;
    /* supermasks: smallest bit length first, largest value first */
    unsigned L0 = tm_bl(a);
    if (L0 == 0) L0 = 1;
    for (unsigned L = L0; L < 63; L++) {
        uint64_t full = (((uint64_t)1) << L) - 1;
        uint64_t m = full & ~a;
        if (L > tm_bl(a)) m &= ~(((uint64_t)1) << (L - 1));
        if (L > tm_bl(a) + 0 || 1) {
            tm_ensure_used(full);
            uint64_t t = 0;
            for (;;) {
                uint64_t k = full ^ t;
                if (k != 0 && !tm_used[k]) return k;
                if (t == m) break;
                t = (t - m) & m;
            }
        }
    }
    return 0;
}

seq_t a(idx_t n)
{
    if (tm_len == 0) {
        tm_cap = 1024;
        tm_terms = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)tm_cap);
        tm_ensure_used(1023);
        tm_terms[0] = 0;
        tm_used[0] = 1;
        tm_len = 1;
    }
    while (tm_len <= n) {
        uint64_t v = tm_next(tm_terms[tm_len - 1]);
        tm_ensure_used(v);
        tm_used[v] = 1;
        if (tm_len >= tm_cap) {
            tm_cap *= 2;
            tm_terms = (uint64_t *)realloc(tm_terms, sizeof(uint64_t) * (size_t)tm_cap);
        }
        tm_terms[tm_len++] = v;
    }
    return (seq_t)tm_terms[n];
}
