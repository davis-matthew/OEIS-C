/*
 * A244557  Numbers obtained by concatenating the squares of the digits of
 *          prime(n).
 * https://oeis.org/A244557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concat_of_squares_of_digits(prime(n)) a(n) =
 *            A048385(A000040(n))
 * Range:     u8 n <= 9, u16 n <= 68, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_prime_digit_squares, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/compute_prime_digit_squares.h"

/* OEIS function definition */
seq_t a(idx_t n)
{
    if (n == 0)                 /* undefined for n < 1, return 0 as a safe default */
        return 0;
    uint32_t p = get_prime(n);  /* p = prime(n) */
    return compute_prime_digit_squares(p);
}
