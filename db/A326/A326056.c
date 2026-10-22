/*
 * A326056  a(n) = gcd(sigma(n)-A008833(n)-n, n-A008833(n)), where sigma is
 *          the sum of divisors of n, and A008833 is the largest square
 *          dividing n.
 * https://oeis.org/A326056
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(sigma(n)-A008833(n)-n, n-A008833(n))
 * Range:     u8 n <= 18, u16 n <= 2082, u32 n <= 9867, u64 n <= 9867,
 *            u128 n <= 9867, big any n
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/factor_u64.h"

/* Computes seq_t a(idx_t n)
   n >= 1, idx_t is unsigned (uint64_t), seq_t is the build's
   integer type (uint8_t … __int128, or arbitrary‑precision). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* below the offset, undefined */

    /* Convert to seq_t once */
    seq_t n_seq = (seq_t)n;

    /* Factor n: max 64 prime factors for n < 2^64 */
    uint64_t primes[64], exps[64];
    size_t cnt;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    /* Compute sigma(n) and largest square divisor */
    seq_t sigma = 1;
    seq_t sq    = 1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];

        /* sigma contribution: (p^(e+1)-1)/(p-1)  */
        seq_t pow_p = 1;
        for (uint64_t k = 0; k < e + 1; ++k) pow_p *= (seq_t)p;
        seq_t term = (pow_p - 1) / (seq_t)(p - 1);
        sigma *= term;

        /* square contribution: p^(2*floor(e/2)) */
        uint64_t twice = (e >> 1) << 1;
        if (twice) {
            seq_t pow_sq = 1;
            for (uint64_t k = 0; k < twice; ++k) pow_sq *= (seq_t)p;
            sq *= pow_sq;
        }
    }

    /* t = n - square_part */
    seq_t t = n_seq - sq;

    /* diff_abs = |sigma - (square + n)| */
    seq_t sum     = sq + n_seq;
    seq_t diff_abs = (sigma >= sum) ? sigma - sum : sum - sigma;

    return gcd(diff_abs, t);
}
