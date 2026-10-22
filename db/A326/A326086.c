/*
 * A326086  E.g.f.: Sum_{n>=0} 2^(n^2) * W(x)^(2^n) * x^n/n!, where W(x) =
 *          LambertW(-x)/(-x).
 * https://oeis.org/A326086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * 2^(k*(k+1)) * (2^k +
 *            n-k)^(n-k-1). For k = n this term simplifies to 2^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;  /* a(0) = 1 */

    /* Pre‑compute powers of 2: 2^i for i = 0 .. 2*n. */
    size_t size = 2 * (size_t)n + 1;          /* indices 0 .. 2*n */
    seq_t *pow2 = (seq_t*)malloc(size * sizeof(seq_t));
    if (!pow2) return 0;                     /* allocation failure */

    pow2[0] = (seq_t)1;
    for (size_t i = 1; i < size; ++i)
        pow2[i] = pow2[i - 1] * (seq_t)2;

    seq_t sum = 0;
    seq_t binom = 1;                    /* C(n,0) */
    seq_t pow2_k_kp1 = 1;               /* 2^(k*(k+1)) for current k */

    for (idx_t k = 0; k < n; ++k) {
        seq_t base = pow2[k] + (seq_t)(n - k);  /* 2^k + n - k */
        int exp = (int)(n - k - 1);             /* n - k - 1 >= 0 for k < n */

        seq_t term = binom;
        term = term * pow2_k_kp1;
        term = term * pow_seq(base, exp);

        sum = sum + term;

        /* Update for the next k */
        if (k + 1 < n) {
            /* C(n,k+1) = C(n,k) * (n-k) / (k+1) */
            seq_t tmp = binom * (seq_t)(n - k);
            binom = tmp / (seq_t)(k + 1);

            /* 2^((k+1)*(k+2)) = 2^(k*(k+1)) * 2^(2*(k+1)) */
            pow2_k_kp1 = pow2_k_kp1 * pow2[2 * (k + 1)];
        }
    }

    /* k = n term: 2^(n^2) */
    int exp_n2 = (int)(n * n);
    sum = sum + pow_seq((seq_t)2, exp_n2);

    free(pow2);
    return sum;
}
