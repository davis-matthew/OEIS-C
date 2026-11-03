/*
 * A326125  Expansion of Sum_{k>=1} k^2 * x^k / (1 + x^k)^2.
 * https://oeis.org/A326125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 224, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 70 terms through n = 9731 (2026-11-01)
 * Helpers:   ipow_u64_idx, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n * A000593(n)
 * where A000593(n) = Σ_{d|n} (-1)^{n/d+1} d.
 * The sequence is multiplicative with
 *    a(2^e)       = 2^e
 *    a(p^e) (p>2) = p^e * (p^{e+1}-1)/(p-1)
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/ipow_u64_idx.h"
#include "../utils/smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* offset protection */

    seq_t result = 1;
    idx_t m = n;

    while (m > 1)
    {
        uint64_t p = smallest_prime_factor(m);
        if (p == 0)
            p = m;                  /* fallback if the helper fails */

        int e = 0;
        while (m % p == 0)
        {
            m /= p;
            ++e;
        }

        seq_t part;
        if (p == 2)
        {
            /* a(2^e) = 2^e */
            part = ipow_u64_idx(2, (idx_t)e);
        }
        else
        {
            /* a(p^e) = p^e * (p^{e+1}-1)/(p-1) for odd primes */
            seq_t pe   = ipow_u64_idx(p, (idx_t)e);
            seq_t pe1  = ipow_u64_idx(p, (idx_t)(e + 1));
            seq_t sum  = (pe1 - 1) / ((seq_t)p - 1);
            part = pe * sum;
        }

        result *= part;
    }

    return result;
}
