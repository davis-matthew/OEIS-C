/*
 * A277097  a(n) = 5 - (prime(n) mod 10).
 * https://oeis.org/A277097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5 - (prime(n) mod 10)
 * Range:     i8 n <= 71, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* obtain the n‑th prime (n >= 1) */
    uint32_t p = get_prime(n);

    /* compute (prime mod 10) and subtract from 5 */
    seq_t mod  = (seq_t)(p % 10);
    return (seq_t)5 - mod;
}
