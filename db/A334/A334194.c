/*
 * A334194  a(n) = n - d1*d2, where d1, d2 are the distances from n to the
 *          previous and the next prime number respectively.
 * https://oeis.org/A334194
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n – d1*d2, d1 = n − previous_prime(n), d2 = next_prime(n)
 *            − n
 * Range:     i8 n <= 135, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3; behaviour for smaller n is irrelevant. */
    if (n < 3) return 0;

    /* Find the largest prime strictly smaller than n. */
    idx_t p_prev = n - 1;
    while (p_prev >= 2 && !is_prime(p_prev))
        p_prev--;
    if (p_prev < 2) /* should not happen for n ≥ 3 */
        return 0;
    idx_t d1 = n - p_prev;

    /* Find the smallest prime strictly greater than n. */
    idx_t p_next = n + 1;
    while (!is_prime(p_next))
        p_next++;
    idx_t d2 = p_next - n;

    /* Return n – d1*d2 (all in seq_t to avoid overflow of intermediate values). */
    return (seq_t)n - (seq_t)d1 * (seq_t)d2;
}
