/*
 * A365339  Length of the longest subsequence of 1,...,n on which the Euler
 *          totient function phi A000010 is nondecreasing.
 * https://oeis.org/A365339
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = length of the longest nondecreasing subsequence of the
 *            sequence φ(1), φ(2), … , φ(n)
 * Range:     u8 n <= 783, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9936 (2026-11-03)
 * Helpers:   mem, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n+1) is either a(n) or a(n)+1;  the algorithm below
    computes a(n) by an O(n log n) longest nondecreasing subsequence
    (LNDS) routine using only integer operations.  */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/totient.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; n=0 is unused */

    /* allocate an array that can hold up to n tails of the LNDS */
    seq_t *tails = mem_new(seq_t, (size_t)n);
    size_t len = 0;                  /* current number of tails */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t x = totient(i);            /* value φ(i) */

        /* binary search for the first tail > x (upper_bound) */
        size_t lo = 0, hi = len;
        while (lo < hi) {
            size_t mid = (lo + hi) >> 1;
            if (tails[mid] > x) hi = mid;
            else                lo = mid + 1;
        }

        /* if lo == len, we extend the LNDS; otherwise we replace a tail */
        if (lo == len) {
            tails[len++] = x;
        } else {
            tails[lo] = x;
        }
    }

    mem_free(tails);                     /* release the temporary buffer */
    return (seq_t)len;               /* length of the longest nondecreasing subsequence */
}
