/*
 * A367857  a(n) is the smallest base b such that (b+1)^n in base b is a
 *          palindrome.
 * https://oeis.org/A367857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2 for n=1,2,3; otherwise a(n) = binomial(n, floor(n/2)) +
 *            1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n <= 3) {
        return (seq_t)2;
    }
    idx_t k = n / 2;          /* floor(n/2) */
    seq_t res = binomial(n, k);
    return res + (seq_t)1;
}
