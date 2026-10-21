/*
 * A278347  If n is even, a(n) = a(n/2 -1) - a(n/2 +1), and if n is odd, a(n)
 *          = a((n-1)/2) - a((n+1)/2), with a(1) = a(2) = 1.
 * https://oeis.org/A278347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A278347
 * a(1) = a(2) = 1
 * a(n) = a(n/2 - 1) - a(n/2 + 1)   if n is even
 *        a((n-1)/2)  - a((n+1)/2)   if n is odd
 *
 * The implementation uses memoised recursion.  The cache is grown on
 * demand and new entries are zero‑initialised regardless of whether the
 * underlying storage moved.  The algorithm runs in O(log n) per call, so
 * it easily handles n up to 10 000 within the required time limit.
 */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static seq_t *cache = NULL;         /* fits a(n) values */
static uint8_t *known = NULL;       /* 1 = value stored, 0 = not yet computed */
static size_t cap = 0;              /* current capacity of the arrays */

/* Ensure that cache[0 .. n] exists; extend and zero new parts as needed. */
static void ensure_capacity(idx_t n)
{
    if (cap <= n) {
        size_t newcap = cap ? cap * 2 : 8;
        while (newcap <= n) newcap <<= 1;

        seq_t *nc = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        uint8_t *nk = (uint8_t *)realloc(known, newcap * sizeof(uint8_t));
        if (!nc || !nk) abort();

        /* Zero the newly allocated part (independent of realloc movement). */
        memset(nc + cap, 0, (newcap - cap) * sizeof(seq_t));
        memset(nk + cap, 0, (newcap - cap));

        cache = nc;
        known = nk;
        cap = newcap;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (known[n]) return cache[n];

    /* Base cases */
    if (n == 1 || n == 2) {
        cache[1] = 1;
        cache[2] = 1;
        known[1] = 1;
        known[2] = 1;
        return 1;
    }

    idx_t i1, i2;
    if (n & 1U) {            /* odd */
        i1 = (n - 1) / 2;
        i2 = (n + 1) / 2;
    } else {                 /* even */
        i1 = n / 2 - 1;
        i2 = n / 2 + 1;
    }

    seq_t v1 = a(i1);
    seq_t v2 = a(i2);
    seq_t res = v1 - v2;

    cache[n] = res;
    known[n] = 1;
    return res;
}
