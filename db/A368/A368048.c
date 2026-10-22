/*
 * A368048  a(n) = lcm_{p in Partitions(n)} (Product_{t in p}(t + m)), where m
 *          = 2.
 * https://oeis.org/A368048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = lcm_{p in Partitions(n)} prod_{t in p} (t + 2) The value
 *            can be computed as a(n) = ∏_{q prime ≤ n+2} q^{e_q} where e_2 =
 *            n - (n mod 2) // even numbers get 2^n, odd get 2^{n-1} e_3 = n
 *            e_q = ⌊ n / (q – 2) ⌋ for q > 3 (and q ≤ n+2) All other primes
 *            have exponent 0. The following implementati...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   build_primes, pow_seq, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/build_primes.h"   /* build_primes */
#include "../utils/pow_seq.h"        /* pow_seq */
#include "../utils/seq_mul.h"        /* seq_mul */

seq_t a(idx_t n)
{
    /* Handle n = 0 explicitly – the loop below works as is, but this
       returns 1 immediately. */
    if (n == 0) return (seq_t)1;

    /* Generate all primes up to n+2. */
    idx_t        *primes = NULL;
    size_t         prime_cnt = 0;
    build_primes(n + 2, &primes, &prime_cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        idx_t exp = 0;

        if (p == 2) {
            /* 2 contributes the factor 2^{n - (n mod 2)}. */
            exp = (n & 1) ? n - 1 : n;
        } else if (p == 3) {
            exp = n;
        } else {
            if (p > n + 2) continue;          /* safety, should not happen */
            idx_t denom = p - 2;
            if (denom > n) continue;           /* floor gives 0 */
            exp = n / denom;                   /* floor division */
        }

        if (exp == 0) continue;

        seq_t term = pow_seq((seq_t)p, (int)exp);
        result = seq_mul(result, term);
    }

    free(primes);
    return result;
}
