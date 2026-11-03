/*
 * A303543  Number of ways to write n as a^2 + b^2 + C(k) + C(m) with 0 <= a
 *          <= b and 0 < k <= m, where C(k) denotes the Catalan number
 *          binomial(2k,k)/(k+1).
 * https://oeis.org/A303543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9494 (2026-11-03)
 * Helpers:   catalan, ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function counts the number of ways to write a positive integer n
   as                                                  a^2 + b^2 + C(k) + C(m)
   with                                           0 ≤ a ≤ b
                                    and                           1 ≤ k ≤ m
   where C(x) denotes the Catalan number binomial(2x,x)/(x+1).
   This is a brute‑force search that runs in time proportional to
   (number of Catalan numbers ≤ n)^2 × √n.  It uses the utility
   functions catalan() and floor_sqrt() provided by the build.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/catalan.h"
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find the largest k such that C(k) ≤ n. */
    idx_t max_k = 0;
    while (1) {
        seq_t c = catalan(max_k + 1);   /* catalan(1) = 1, … */
        if (c > (seq_t)n) break;
        ++max_k;
    }

    /* Pre‑compute Catalan numbers up to max_k. */
    seq_t *C = mem_new(seq_t, (max_k + 1));
    if (!C) return 0;                       /* Allocation failed – return 0 */
    for (idx_t i = 0; i <= max_k; ++i)
        C[i] = catalan(i);

    seq_t count = 0;
    seq_t nseq  = (seq_t)n;

    /* Enumerate all pairs (k,m) with 1 ≤ k ≤ m ≤ max_k. */
    for (idx_t k = 1; k <= max_k; ++k) {
        seq_t Ck = C[k];
        if (Ck > nseq) break;                 /* larger k will only grow Ck */

        for (idx_t m = k; m <= max_k; ++m) {
            seq_t Cm = C[m];
            seq_t S  = Ck + Cm;
            if (S > nseq) break;              /* larger m will only increase S */

            seq_t T = nseq - S;               /* remaining value for a^2 + b^2 */

            /* Maximum possible value of a is floor_sqrt(T). */
            idx_t maxA = (idx_t)floor_sqrt(T);

            for (idx_t a = 0; a <= maxA; ++a) {
                seq_t a2   = (seq_t)a * (seq_t)a;
                seq_t rem  = T - a2;
                seq_t b    = floor_sqrt(rem);  /* integer square root of rem */

                if (b * b == rem && b >= (seq_t)a)
                    ++count;
            }
        }
    }

    mem_free(C);
    return count;
}
