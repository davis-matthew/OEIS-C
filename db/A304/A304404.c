/*
 * A304404  If n = Product (p_j^k_j) then a(n) = Product (n/p_j^k_j).
 * https://oeis.org/A304404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p^k||n} n / p^k (equivalently n^(ω(n)-1) for n>1,
 *            a(1)=1)
 * Range:     u8 n <= 29, u16 n <= 209, u32 n <= 1637, u64 n <= 8738,
 *            u128 n <= 8738, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;            /* a(1) = 1 */

    seq_t result = (seq_t)1;        /* product accumulator */
    idx_t temp   = n;               /* value to factorize */

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            /* pk = p^k, the highest power of p dividing n */
            idx_t pk = 1;
            while (temp % p == 0) {
                pk *= p;
                temp /= p;
            }
            result *= (seq_t)(n / pk);   /* multiply by n/p^k */
        }
    }

    /* if a prime factor > sqrt(n) remains */
    if (temp > 1) {
        result *= (seq_t)(n / temp);    /* temp is a prime, exponent 1 */
    }

    return result;
}
