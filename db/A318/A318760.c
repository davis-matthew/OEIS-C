/*
 * A318760  a(n) = (2^(p-2) + 3^(p-2) + 6^(p-2) - 1)/p where p = prime(n).
 * https://oeis.org/A318760
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2^(p-2) + 3^(p-2) + 6^(p-2) - 1)/p where p = prime(n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* For indices below the OEIS offset the value is undefined; return 0. */
    if (n < 3)
        return (seq_t)0;

    /* Retrieve the n‑th prime. */
    seq_t p = (seq_t)get_prime(n);   /* prime(n) ≥ 5 for n ≥ 3 */

    /* Exponent: p – 2. */
    seq_t exp = p - (seq_t)2;

    /* Compute the three powers. */
    seq_t t2 = big_pow((seq_t)2, exp);
    seq_t t3 = big_pow((seq_t)3, exp);
    seq_t t6 = big_pow((seq_t)6, exp);

    /* Numerator: 2^(p-2) + 3^(p-2) + 6^(p-2) – 1. */
    seq_t num = t2 + t3 + t6;
    num -= (seq_t)1;

    /* Division is exact because p divides the numerator by FLT. */
    return num / p;
}
