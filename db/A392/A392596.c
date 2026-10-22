/*
 * A392596  a(n) = A276086(n) mod 360, where A276086 is the primorial base
 *          exp-function.
 * https://oeis.org/A392596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = A276086(n) mod 360
 * Range:     u8 n <= 14, u16 n <= 9979, u32 n <= 9979, u64 n <= 9979,
 *            u128 n <= 9979, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A276086(n) is the primorial‑base exponent function:
   write n in the primorial base
     n = Σ d_k · P_k   (P_k = ∏_{i<k} p_i, p_0 = 2)
   where 0 ≤ d_k < p_k,
   then A276086(n) = ∏ p_k^{d_k}.  a(n) is this value modulo 360. */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* A276086(0) = 1, 1 mod 360 = 1 */

    /* Small static array to hold primes and primorials.  */
    /* The size 32 is more than enough for all n used in the tests. */
    uint64_t primes[32];
    size_t pc = 0;                     /* number of primes found          */
    seq_t primorial = (seq_t)1;        /* P_0 = 1                          */

    uint64_t cand = 2;                 /* next integer to test for prime   */

    while (primorial <= (seq_t)n && pc < 32)
    {
        /* find the next prime */
        while (!is_prime(cand)) cand++;
        primes[pc++] = cand;

        /* update primorial:  P_{pc} = P_{pc-1} · p_{pc-1}                 */
        primorial = primorial * (seq_t)primes[pc-1];

        cand++;
    }

    /* Build the array of primorials:  P[0] = 1,  P[i+1] = P[i] * p[i]   */
    seq_t prims[33];
    prims[0] = (seq_t)1;
    for (size_t i = 0; i < pc; ++i)
        prims[i+1] = prims[i] * (seq_t)primes[i];

    /* Compute the product of primes raised to the corresponding digits
       modulo 360.                                                     */
    seq_t result = (seq_t)1;
    seq_t remaining = (seq_t)n;

    /* Loop over indices from pc-1 down to 0.  */
    for (size_t i = pc; i-- > 0; )
    {
        seq_t digit = remaining / prims[i];
        remaining = remaining % prims[i];

        /* digit < primes[i]  (by definition of primorial base digits) */
        seq_t powmod = mod_pow((seq_t)primes[i], (uint64_t)digit, (seq_t)360);

        result = (result * powmod) % (seq_t)360;
    }

    return result;
}
