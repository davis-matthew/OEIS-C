/*
 * A308780  First element of the periodic part of the continued fraction
 *          expansion of sqrt(k), where the period is 2.
 * https://oeis.org/A308780
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For each integer k≥2 whose continued‑fraction expansion of
 * sqrt(k) has period length exactly 2, find the first periodic partial
 * quotient a1.  The function returns the n‑th such a1, where n has the
 * OEIS offset of 1.  Thus a(1) corresponds to k=3 (period 2, a1=1),
 * a(2) to k=6 (a1=2), a(3) to k=8 (a1=1), etc. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square root (floor) for uint64_t.  Uses 128‑bit arithmetic to
 * avoid overflow. */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1ULL << 32;  /* high > sqrt(UINT64_MAX) */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * mid <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Return the n‑th term of the sequence. */
seq_t a(idx_t n)
{
    /* Static cache of results. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity of cache   */
    static size_t sz    = 0;   /* number of values stored       */
    static idx_t next_k = 2;   /* next integer to examine (k≥2) */

    /* Expand the cache until it contains at least n elements. */
    while (sz < n) {
        idx_t k = next_k++;          /* candidate k */

        /* Skip perfect squares. */
        idx_t a0 = _isqrt_u64(k);
        if (a0 * a0 == k)
            continue;

        /* First iteration: compute d1 and a1.  Use 128‑bit arithmetic
         * to avoid overflow in the product 2*a0. */
        idx_t d1 = k - a0 * a0;           /* d1 > 0 */
        if (d1 == 0)
            continue;                     /* safety (should not happen) */

        /* a1 = floor((2*a0) / d1).  Use 128‑bit to avoid overflow. */
        seq_t a1 = (seq_t)((unsigned __int128)2 * a0 / d1);

        /* If a1 equals 2*a0 then the period length is 1, skip. */
        if (a1 == (seq_t)2 * a0)
            continue;

        /* Second iteration to test for period length 2. */
        /* m2 = d1*a1 - a0.  Compute with 128‑bit to stay safe. */
        idx_t m2 = (idx_t)((unsigned __int128)d1 * a1 - a0);

        /* sub = k - m2*m2. */
        unsigned __int128 sub128 = (unsigned __int128)k -
                                   (unsigned __int128)m2 * m2;
        /* d2 = sub / d1.  sub should be divisible by d1. */
        if (sub128 % d1 != 0)
            continue;                     /* not a perfect division */
        idx_t d2 = (idx_t)(sub128 / d1);

        if (d2 == 0)                      /* safety */
            continue;

        /* a2 = floor((a0 + m2) / d2). */
        seq_t a2 = (seq_t)(a0 + m2) / d2;

        /* Accept only if a2 == 2*a0 (period length exactly 2). */
        if (a2 != (seq_t)2 * a0)
            continue;

        /* Store the a1 value. */
        if (sz == cap) {
            size_t newcap = (cap == 0) ? 16 : cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp)   /* allocation failure: return 0 as a fallback */
                return (seq_t)0;
            cache = tmp;
            cap   = newcap;
        }
        cache[sz++] = a1;
    }

    return cache[n - 1];
}
