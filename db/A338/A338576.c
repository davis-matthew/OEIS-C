/*
 * A338576  a(n) = n * pod(n) where pod(n) = the product of divisors of n
 *          (A007955).
 * https://oeis.org/A338576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * pod(n) where pod(n) = product of all divisors of n
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"

/* Integer exponentiation for seq_t using repeated squaring.
   Uses seq_mul from the utilities. */
static seq_t seq_pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1U)
            result = seq_mul(result, base);
        exp >>= 1U;
        if (exp)
            base = seq_mul(base, base);
    }
    return result;
}

/* Compute a(n) = n * pod(n) .
   pod(n) = product of all positive divisors of n.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined part of the sequence */

    /* Special‑case n==1 to avoid the factorisation loop. */
    if (n == 1)
        return 1;

    /* Factorise n by trial division – sufficient for n ≤ 10 000. */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t pcnt = 0;

    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            uint64_t e = 0;
            while (t % p == 0)
            {
                t /= p;
                ++e;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            ++pcnt;
        }
    }
    if (t > 1)
    {
        primes[pcnt] = t;
        exps[pcnt]   = 1;
        ++pcnt;
    }

    /* Compute d(n) = ∏ (e_i + 1).  */
    uint64_t d = 1;
    for (size_t i = 0; i < pcnt; ++i)
        d *= (exps[i] + 1);

    /* Compute pod(n) = ∏ p_i ^ (e_i * d / 2).  */
    seq_t pod = 1;
    for (size_t i = 0; i < pcnt; ++i)
    {
        idx_t exp = (idx_t)(exps[i] * d / 2ULL);   /* integer division */
        seq_t term = seq_pow_seq((seq_t)primes[i], exp);
        pod = seq_mul(pod, term);
    }

    /* a(n) = n * pod(n). */
    return seq_mul((seq_t)n, pod);
}
