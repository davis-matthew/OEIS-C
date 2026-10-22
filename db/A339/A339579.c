/*
 * A339579  a(n) = least nonnegative integer k such that n*2^k - 1 is
 *          composite.
 * https://oeis.org/A339579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 9861, u32 n <= 9861, u64 n <= 9861,
 *            u128 n <= 9861, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: least k such that n*2^k - 1 is composite. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    seq_t m = (seq_t)n - 1;   /* m_k = n*2^k - 1, start with k=0 */
    idx_t k = 0;

    while (1) {
        if (m > 1 && !is_prime_big(m)) {
            return (seq_t)k;
        }
        m = 2 * m + 1;  /* m_{k+1} = 2*m_k + 1 */
        k++;
    }
}
