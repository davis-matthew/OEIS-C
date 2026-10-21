/*
 * A248003  a(n) = (sum of totatives of n ) / (2^(omega(n)-1)); a(n) =
 *          A023896(n) / A007875(n).
 * https://oeis.org/A248003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 366, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64, pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A023896(n) / A007875(n)
 *          = n * φ(n) / 2^ω(n)
 *          = sum of totatives of n divided by 2^(ω(n)-1)
 *          (where φ is Euler's totient and ω is the number of distinct primes)
 */

#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/pow2.h"

/* Compute A248003: a(n) = (sum of totatives of n) / (2^(omega(n)-1)) */
seq_t a(idx_t n)
{
    if (n == 1) /* A023896(1)=1, A007875(1)=1 by convention */
        return (seq_t)1;

    /* Factor n: n = Π p_i^e_i */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt; /* number of distinct prime factors = ω(n) */
    factor_u64(n, pr, ex, &cnt);

    /* Compute φ(n) = Π (p_i-1) * p_i^(e_i-1) */
    seq_t phi = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t t = pow_seq((seq_t)p, (int)(e - 1));        /* p^(e-1) */
        seq_t term = (seq_t)(p - 1) * t;                 /* (p-1)*p^(e-1) */
        phi = phi * term;
    }

    /* a(n) = n * φ(n) / 2^ω(n) */
    seq_t numerator = (seq_t)n * phi;
    seq_t denom     = pow2((idx_t)cnt);
    return numerator / denom;
}
