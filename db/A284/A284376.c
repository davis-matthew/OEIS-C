/*
 * A284376  a(n) is the least nonnegative integer such that n + i*a(n) is a
 *          Gaussian prime.
 * https://oeis.org/A284376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least nonnegative integer a such that n + i·a is a
 *            Gaussian prime
 * Range:     u8 n <= 15, u16 n <= 373, u32 n <= 373, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Special case: n = 0 → the first Gaussian prime on the imaginary axis is 3i. */
    if (n == 0)
        return (seq_t)3;

    /* Promote the index to the result type. */
    seq_t sn = (seq_t)n;

    /* If n itself is a rational prime ≡ 3 (mod 4), it is a Gaussian prime on the real axis. */
    if (is_prime_big(sn) && ((n & 3) == 3))
        return (seq_t)0;

    /* For a > 0, n + i·a is Gaussian prime ⇔ N(n+i·a)=n²+a² is a rational prime. */
    for (idx_t a_val = 1;; ++a_val) {
        seq_t sum = sn * sn + (seq_t)a_val * (seq_t)a_val;
        if (is_prime_big(sum))
            return (seq_t)a_val;
    }
}
