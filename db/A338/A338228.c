/*
 * A338228  Number of numbers less than or equal to n whose square does not
 *          divide n.
 * https://oeis.org/A338228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - tau(sqrt(m)) where m is the largest square dividing n
 *            Equivalently, a(n) counts the numbers k ≤ n such that k² does
 *            not divide n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n) {
    // count square divisors: product over prime factors of (floor(e/2)+1)
    seq_t tau = 1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            int cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            int half = cnt / 2;              // floor(e/2)
            tau *= (half + 1);
        }
    }
    if (temp > 1) {
        // remaining prime factor has exponent 1 -> floor(1/2)=0, multiplier 1
        // (do nothing)
    }
    seq_t ans = (seq_t)n - tau;
    return ans;
}
