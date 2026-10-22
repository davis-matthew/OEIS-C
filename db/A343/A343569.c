/*
 * A343569  If n = Product (p_j^k_j) then a(n) = Product (2*(p_j^k_j + 1)),
 *          with a(1) = 1.
 * https://oeis.org/A343569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Π_{p^k‖n} 2 * (p^k + 1) (for n ≥ 1, a(1) = 1)
 * Range:     u8 n <= 29, u16 n <= 1769, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* handle factor 2 */
    if ((temp & 1) == 0) {
        idx_t k = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++k;
        }
        seq_t pk = 1;
        for (idx_t i = 0; i < k; ++i) pk *= (seq_t)2;
        result *= (seq_t)2 * (pk + (seq_t)1);
    }

    /* handle odd prime factors */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t k = 0;
            while (temp % p == 0) {
                temp /= p;
                ++k;
            }
            seq_t pk = 1;
            for (idx_t i = 0; i < k; ++i) pk *= (seq_t)p;
            result *= (seq_t)2 * (pk + (seq_t)1);
        }
    }

    /* remaining prime factor greater than sqrt(original n) */
    if (temp > 1) {
        seq_t pk = (seq_t)temp;          /* p^1 */
        result *= (seq_t)2 * (pk + (seq_t)1);
    }

    return result;
}
