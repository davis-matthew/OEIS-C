/*
 * A343759  Total area of all p X r rectangles where n = p + r, p < r, p is
 *          prime and r is a positive integer.
 * https://oeis.org/A343759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..floor((n-1)/2)} k*(n-k)*c(k) where c(k)=1 iff k
 *            is prime
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t limit = (n - 1) / 2;
    for (idx_t k = 2; k <= limit; ++k) {
        if (is_prime(k)) {
            seq_t kk = (seq_t)k;
            seq_t rr = (seq_t)(n - k);
            sum += kk * rr;
        }
    }
    return sum;
}
