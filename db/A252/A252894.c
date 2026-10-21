/*
 * A252894  Primes that are congruent to both 11 mod 111 and 111 mod 1111.
 * https://oeis.org/A252894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A search for the n-th prime p with p ≡ 12332 (mod 123321) */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Offset 1: return 0 for n == 0 to avoid undefined behaviour */
    if (n == 0)
        return 0;

    const seq_t REM = (seq_t)12332;   /* desired remainder */
    const seq_t STEP = (seq_t)123321; /* modulus */

    idx_t count = 0;
    for (idx_t k = 0; ; ++k)
    {
        seq_t candidate = REM + STEP * (seq_t)k;
        if (is_prime_big(candidate))
        {
            ++count;
            if (count == n)
                return candidate;
        }
    }
}
