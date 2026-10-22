/*
 * A391960  a(n) = Sum_{k=0..floor(n/3)} (k+1) * 2^k * 3^(n-3*k) *
 *          binomial(k,2*(n-3*k)).
 * https://oeis.org/A391960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/3⌋} (k+1)*2^k*3^(n-3k)*binomial(k,2*(n-3k))
 * Range:     u8 n <= 9, u16 n <= 22, u32 n <= 41, u64 n <= 83, u128 n <= 168,
 *            big any n
 * Helpers:   binomial, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    idx_t max_k = n / 3;          /* floor(n/3) */
    seq_t sum   = 0;

    for (idx_t k = 0; k <= max_k; ++k)
    {
        idx_t rem = n - 3 * k;              /* n-3k */
        idx_t choose = 2 * rem;             /* 2*(n-3k) */
        if (choose > k)                     /* binomial will be zero */
            continue;

        seq_t term = binomial(k, choose);   /* binomial(k,2*(n-3k)) */

        /* multiply by (k+1) */
        term *= (seq_t)(k + 1);

        /* multiply by 2^k */
        term *= pow2(k);

        /* multiply by 3^(n-3k) */
        term *= pow3(rem);

        sum += term;
    }

    return sum;
}
