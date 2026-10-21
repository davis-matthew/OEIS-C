/*
 * A263399  Number of ordered pairs (k, m) with 0 < m < n and |k| < prime(m)^2
 *          such that (k+prime(m+1)^2)/(k+prime(m)^2) =
 *          (k+prime(n+1)^2)/(k+prime(n)^2).
 * https://oeis.org/A263399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 150, u16 n <= 150, u32 n <= 150, u64 n <= 150,
 *            u128 n <= 150, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A263399: number of ordered pairs (k,m) with 0 < m < n and
   |k| < prime(m)^2 such that (k+prime(m+1)^2)/(k+prime(m)^2)
   = (k+prime(n+1)^2)/(k+prime(n)^2).  The function enumerates m
   and tests the Diophantine equation. */
   
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t total = 0;                   /* result accumulator */
    if (n <= 1) return 0;              /* undefined for n < 2, return 0 */

    /* Precompute primes that are independent of m */
    uint64_t pn   = get_prime(n);
    uint64_t pn1  = get_prime(n + 1);
    __int128 pn_sq  = (__int128)pn  * pn;
    __int128 pn1_sq = (__int128)pn1 * pn1;

    for (idx_t m = 1; m < n; ++m)
    {
        uint64_t pm   = get_prime(m);
        uint64_t pm1  = get_prime(m + 1);
        __int128 pm_sq  = (__int128)pm  * pm;
        __int128 pm1_sq = (__int128)pm1 * pm1;

        /* A = p_n^2 + p_{m+1}^2 - p_m^2 - p_{n+1}^2 */
        __int128 A = pn_sq + pm1_sq - pm_sq - pn1_sq;

        /* B = p_{n+1}^2*p_m^2 - p_{m+1}^2*p_n^2 */
        __int128 B = pn1_sq * pm_sq - pm1_sq * pn_sq;

        if (A == 0)
        {
            if (B == 0)                      /* any k satisfies the equation */
            {
                /* Count all k with |k| < p_m^2   */
                uint64_t limit = (uint64_t)pm_sq;   /* p_m^2 fits in 64 bits */
                total += (seq_t)(2 * limit - 1);
            }
            continue;
        }

        if (B % A != 0) continue;            /* k must be integer */

        __int128 k = B / A;
        __int128 abs_k = k < 0 ? -k : k;

        if (abs_k < pm_sq)                  /* |k| < p_m^2  */
        {
            total += 1;
        }
    }
    return total;
}
