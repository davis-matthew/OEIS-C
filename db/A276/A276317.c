/*
 * A276317  a(n) = b(n)/c(n) where b(n) = smallest positive k such that
 *          (2*k)^2 + 2*n - 1 is prime and c(n) = gcd(n,3) = A109007(n).
 * https://oeis.org/A276317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k / gcd(k,3) where k is the smallest positive integer
 *            such that (2*k)^2 + 2*n – 1 is prime. (This matches OEIS
 *            A276317.)
 * Range:     u8 n <= 20, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    seq_t k = 1;
    seq_t two_n = 2 * ((seq_t)n);
    while (1) {
        /* Compute (2*k)^2 + 2*n – 1   = 4*k^2 + 2*n – 1  */
        seq_t val = 4 * k * k + two_n - 1;
        if (is_prime_big(val))
            break;
        ++k;
    }
    /* c = gcd(k,3); for 3 only this is 1 or 3 */
    seq_t c = (k % 3 == 0) ? (seq_t)3 : (seq_t)1;
    return k / c;
}
