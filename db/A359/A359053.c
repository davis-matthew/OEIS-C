/*
 * A359053  a(n) = Sum_{d|n} sigma_d(d)^(n/d).
 * https://oeis.org/A359053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} sigma_d(d)^(n/d)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (σ_k(m) is the sum of the k‑th powers of the divisors of m) */
#include <stddef.h>                   /* size_t */
#include "../utils/big_pow.h"         /* seq_t big_pow(seq_t, seq_t); */

/* Compute A359053:  a(n) = Σ_{d|n} σ_d(d)^(n/d) */
seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1, but guard against 0 */
        return (seq_t)0;

    seq_t result = 0;

    /* Collect divisors of n into a static array */
    /* The maximum number of divisors for n ≤ 10000 is < 200. */
    idx_t divs[256];
    size_t divc = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[divc++] = i;
            if (i != n / i)
                divs[divc++] = n / i;
        }
    }

    /* For each divisor d compute σ_d(d) and raise it to (n/d) */
    for (size_t di = 0; di < divc; ++di) {
        idx_t d = divs[di];

        /* σ_d(d) = Σ_{t|d} t^d */
        seq_t sigma = 0;
        for (idx_t j = 1; j * j <= d; ++j) {
            if (d % j == 0) {
                seq_t p = big_pow((seq_t)j, (seq_t)d);
                sigma += p;
                if (j != d / j) {
                    seq_t q = big_pow((seq_t)(d / j), (seq_t)d);
                    sigma += q;
                }
            }
        }

        /* term = sigma^(n/d) */
        idx_t exp_nd = n / d;
        seq_t term = big_pow(sigma, (seq_t)exp_nd);
        result += term;
    }

    return result;
}
