/*
 * A324798  a(n) = floor(sqrt(2)*prime(n)) - prime(n+1).
 * https://oeis.org/A324798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = floor(sqrt(2) * prime(n)) - prime(n+1)
 * Range:     i8 n <= 3, i16 n <= 24, i32 n <= 76, i64 n <= 76, i128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1; return 0 for safety */
    /* Retrieve the n‑th and (n+1)‑th primes (1‑based). */
    seq_t p_n    = (seq_t)get_prime(n);
    seq_t p_next = (seq_t)get_prime(n + 1);

    /* Compute floor(sqrt(2) * p_n) as floor_sqrt(2 * p_n^2). */
    seq_t square = p_n * p_n;
    square <<= 1; /* multiply by 2 */
    seq_t sqrt_term = floor_sqrt(square);

    return sqrt_term - p_next;
}
