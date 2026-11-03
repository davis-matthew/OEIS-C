/*
 * A353287  a(n) = Sum_{k=0..floor(n/2)} k^k * Stirling2(n-k,k).
 * https://oeis.org/A353287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 24 (2026-11-03)
 * Helpers:   ipow, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A353287: a(n) = Sum_{k=0..floor(n/2)} k^k * Stirling2(n-k,k) */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Computes a(n) for any n >= 0.  The function may be called in any
 * order; it keeps a cached table up to the largest index requested.
 */
seq_t a(idx_t n)
{
    /* Local static cache */
    static seq_t *vals = NULL;          /* a(0)..a(max_n)           */
    static seq_t *prev = NULL;          /* S(m,k-1) for current k   */
    static seq_t *curr = NULL;          /* S(m,k)   for current k   */
    static idx_t  max_n = 0;            /* largest n for which cache was computed */

    /* Recompute cache if needed (initial call or n beyond current cache) */
    if (!vals || n > max_n) {
        idx_t newmax = n;                 /* new maximal index needed */

        /* Allocate fresh arrays of size newmax+1 */
        vals = seq_alloc_new_array(newmax + 1);
        prev = seq_alloc_new_array(newmax + 1);
        curr = seq_alloc_new_array(newmax + 1);
        if (!vals || !prev || !curr)      /* allocation failed – give up */
            return 0;

        /* Initialise a(0)=1, others 0 (will be overwritten) */
        for (idx_t i = 0; i <= newmax; ++i)
            vals[i] = 0;
        vals[0] = 1;

        /* Initial Stirling column S(m,0):  S(0,0)=1, S(m,0)=0 (m>0) */
        for (idx_t i = 0; i <= newmax; ++i)
            prev[i] = 0;
        prev[0] = 1;

        /* Total number of k to consider: floor(newmax/2) */
        idx_t Kmax = newmax / 2;

        for (idx_t k = 1; k <= Kmax; ++k) {
            /* Compute k^k once */
            seq_t k_pow = ipow((seq_t)k, (seq_t)k);

            /* Compute S(m,k) for all m in [0,newmax] */
            for (idx_t m = 0; m <= newmax; ++m) {
                if (m < k) {
                    curr[m] = 0;
                } else if (m == k) {
                    curr[m] = 1;          /* S(k,k)=1 */
                } else {
                    /* S(m,k) = k * S(m-1,k) + S(m-1,k-1) */
                    seq_t term1 = mul_small(curr[m - 1], (unsigned int)k);
                    curr[m] = term1 + prev[m - 1];
                }
            }

            /* Accumulate contributions to a(n) for n >= 2k */
            idx_t start = 2 * k;          /* first n where S(n-k,k) is defined */
            for (idx_t i = start; i <= newmax; ++i) {
                vals[i] += k_pow * curr[i - k];
            }

            /* Prepare for next k: swap prev and curr */
            seq_t *tmp = prev;
            prev = curr;
            curr = tmp;
        }

        /* Cache now contains values up to newmax */
        max_n = newmax;
    }

    /* Return the cached value */
    return vals[n];
}
