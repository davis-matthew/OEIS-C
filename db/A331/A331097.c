/*
 * A331097  a(n) is the greatest prime number of the form n mod (10^k) for
 *          some k > 0, or 0 if no such prime number exists.
 * https://oeis.org/A331097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = greatest prime number of the form n mod (10^k) for some
 *            k>0, or 0 if none
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits_of, is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow10.h"
#include "../utils/digits_of.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)0;
    }
    seq_t n_seq = (seq_t)n;
    int d = digits_of(n);
    for (int k = d; k >= 1; --k) {
        seq_t pow = pow10((idx_t)k);
        seq_t suffix = n_seq % pow;
        if (suffix <= (seq_t)1) {
            continue;
        }
        if (is_prime_big(suffix)) {
            return suffix;
        }
    }
    return (seq_t)0;
}
