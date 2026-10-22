/*
 * A356360  Denominator of the continued fraction
 *          1/(2-3/(3-4/(4-5/(...(n-1)-n/(n+1))))).
 * https://oeis.org/A356360
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-1) if 2n-1 is prime, 1 otherwise (except a(5)=3)
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n) {
    if (n < 3)
        return (seq_t)0;          /* behavior below offset is irrelevant */
    if (n == 5)
        return (seq_t)3;          /* special case */
    seq_t m = (seq_t)n * (seq_t)2 - (seq_t)1;  /* 2n-1 */
    return is_prime_big(m) ? m : (seq_t)1;
}
