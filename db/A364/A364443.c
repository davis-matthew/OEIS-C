/*
 * A364443  a(n) is the number of integers k of the form x^2 + x*y + y^2
 *          (A003136) with n^2 < k < (n+1)^2.
 * https://oeis.org/A364443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 706, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implementation of OEIS A364443
 *  a(n) – number of integers k of the form x²+xy+y² with n² < k < (n+1)².
 *  The function uses a bit‑set to mark all representable numbers up to a
 *  required maximum and counts bits in each interval.
 *  Complexity: O(max_n²) preprocessing, O((2n+1)/64) per call.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------- */
/* integer square root (floor) for 64‑bit values                         */
static inline idx_t int_sqrt_u64(idx_t v) {
    idx_t lo = 0, hi = v;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > v / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/* ------------------------------------------------------------------- */
/* bit‑set storage for representable numbers                           */
struct bitset_t {
    uint64_t *bits;        /* array of 64‑bit words                     */
    idx_t    words;        /* number of words allocated                */
    idx_t    limit;        /* maximum number that can be represented    */
    idx_t    nmax;         /* maximum n precomputed for                    */
};
static struct bitset_t _bs = {NULL, 0, 0, 0};

/* ------------------------------------------------------------------- */
/* ensure bit‑set covers all numbers up to (new_nmax+1)²-1             */
static void ensure_bitset(idx_t new_nmax) {
    if (new_nmax <= _bs.nmax) return;            /* already enough */

    /* free old  (the new buffer will be larger) */
    free(_bs.bits);

    idx_t max_k = (new_nmax + 1) * (new_nmax + 1);
    _bs.limit = max_k;
    _bs.words = (max_k + 63) >> 6;
    _bs.bits  = (uint64_t*)calloc((size_t)_bs.words, sizeof(uint64_t));
    if (_bs.bits == NULL) abort();   /* out of memory */

    /* enumerate all representations x²+xy+y² ≤ max_k   */
    idx_t xmax = int_sqrt_u64(max_k);
    for (idx_t x = 0; x <= xmax; ++x) {
        idx_t x2 = x * x;
        for (idx_t y = 0; y <= x; ++y) {
            idx_t v = x2 + x * y + y * y;
            if (v > max_k) break;
            _bs.bits[v >> 6] |= 1ULL << (v & 63);
        }
    }

    _bs.nmax = new_nmax;
}

/* ------------------------------------------------------------------- */
/* popcount helper                                                      */
static inline idx_t popcount_uint64(uint64_t v) {
    return (idx_t)__builtin_popcountll(v);
}

/* ------------------------------------------------------------------- */
/* main sequence function                                              */
seq_t a(idx_t n)
{
    /* pre‑compute up to this n if necessary */
    if (n > _bs.nmax) {
        /* use a small cushion (e.g., +1000) to reduce reallocations */
        idx_t need = n + 1000;
        if (need < 20000) need = 20000; /* provide a sensible minimum   */
        ensure_bitset(need);
    }

    /* compute bounds of the interval (n², (n+1)²) */
    idx_t low  = n * n + 1;
    idx_t high = (n + 1) * (n + 1) - 1;
    if (low > high) return (seq_t)0;

    idx_t lword = low >> 6, rword = high >> 6;
    int   lbit  = low & 63, rbit = high & 63;
    seq_t count = 0;

    if (lword == rword) {
        uint64_t mask = ((1ULL << (rbit - lbit + 1)) - 1) << lbit;
        count += popcount_uint64(_bs.bits[lword] & mask);
    } else {
        /* left partial word */
        uint64_t maskL = ~0ULL << lbit;
        count += popcount_uint64(_bs.bits[lword] & maskL);

        /* middle full words */
        for (idx_t w = lword + 1; w < rword; ++w)
            count += popcount_uint64(_bs.bits[w]);

        /* right partial word */
        uint64_t maskR = (rbit == 63) ? ~0ULL : ((1ULL << (rbit + 1)) - 1);
        count += popcount_uint64(_bs.bits[rword] & maskR);
    }

    return count;
}
