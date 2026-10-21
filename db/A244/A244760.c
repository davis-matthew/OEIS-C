/*
 * A244760  a(n) = Sum_{k=0..n} C(n,k) * (1 + 3^k)^(n-k) * 2^k.
 * https://oeis.org/A244760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k) * (1 + 3^k)^(n-k) * 2^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow3, pow_seq, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/pow_seq.h"
#include "../utils/pow_uint.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;
    seq_t pow3k   = (seq_t)1;   /* 3^k */
    seq_t two_k   = (seq_t)1;   /* 2^k */

    for (idx_t k = (idx_t)0; k <= n; ++k) {
        if (k > (idx_t)0) {
            pow3k *= (seq_t)3; /* update 3^k */
            two_k *= (seq_t)2; /* update 2^k */
        }

        seq_t binom = binomial(n, k);
        seq_t base  = pow3k + (seq_t)1;            /* 1 + 3^k */
        seq_t pow_exp = pow_seq(base, n - k);      /* (1 + 3^k)^(n-k) */
        seq_t term    = binom * pow_exp * two_k;   /* binomial * ... * 2^k */

        result += term;
    }

    return result;
}
