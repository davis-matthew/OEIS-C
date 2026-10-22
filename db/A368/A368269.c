/*
 * A368269  Expansion of e.g.f. exp(-2*x) / (1 - 2*x*exp(x)).
 * https://oeis.org/A368269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} 2^(n-k) * (n-k-2)^k / k!
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* n! */
    seq_t factN = 1;
    for (idx_t i = 1; i <= n; ++i)
        factN *= (seq_t)i;

    /* 2^n */
    seq_t pow2 = pow_uint(2ULL, n);

    seq_t factDiv = factN;   /* n! / k!  (starts with k=0) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k) {
        int64_t base_int = (int64_t)n - (int64_t)k - 2;
        uint64_t abs_base = (base_int >= 0) ? (uint64_t)base_int
                                            : -(uint64_t)base_int;

        /* (n-k-2)^k */
        seq_t base_pow = pow_uint(abs_base, k);

        /* term = (n! / k!) * 2^(n-k) * abs_base^k */
        seq_t term = factDiv * pow2;
        term *= base_pow;

        /* sign: negative only when base is negative and k is odd */
        if (base_int < 0 && (k & 1))
            sum -= term;
        else
            sum += term;

        /* update for next k */
        if (k < n) {
            factDiv /= (seq_t)(k + 1);  /* exact division by k+1 */
            pow2 /= 2;                  /* exact division by 2 */
        }
    }
    return sum;
}
