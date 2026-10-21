/*
 * A271974  Let p = prime(n): if p mod 4 == 1 then a(n) = (1+p)/2 otherwise if
 *          p mod 4 == 3 then a(n) = (1-p)/2.
 * https://oeis.org/A271974
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (prime(n) mod 4 == 1) ? (1 + prime(n))/2 : (1 -
 *            prime(n))/2
 * Range:     i8 n <= 31, i16 n <= 4080, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based).  The function is safe for n ≥ 2 as required. */
    uint32_t p = get_prime(n);
    seq_t sp = (seq_t)p;

    if (p % 4 == 1)
        return (sp + 1) / 2;
    else
        return (1 - sp) / 2;
}
