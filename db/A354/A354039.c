/*
 * A354039  Odd numbers k for which sigma(k^2) == 1 (mod 4).
 * https://oeis.org/A354039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A354039: odd numbers k for which σ(k²) ≡ 1 (mod 4).  
   Equivalent: the number of prime powers pᵉ with p ≡ 1 (mod 4) and
   e odd is even.  This implementation tests each odd integer and
   counts the accepted ones until the n‑th term is found. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    uint64_t candidate = 1;          /* start with the first odd number */
    size_t   count     = 0;          /* how many terms have been found */
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t   fcnt;

    while (1)
    {
        /* factorise the current candidate */
        factor_u64(candidate, primes_buf, exps_buf, &fcnt);

        /* count p^e where p ≡ 1 (mod 4) and e is odd */
        size_t pcnt = 0;
        for (size_t i = 0; i < fcnt; ++i)
            if ((primes_buf[i] & 3) == 1 && (exps_buf[i] & 1))
                ++pcnt;

        /* accept if that count is even */
        if ((pcnt & 1) == 0)
        {
            ++count;
            if (count == (size_t)n)
                return (seq_t)candidate;
        }

        candidate += 2;   /* next odd integer */
    }
}
