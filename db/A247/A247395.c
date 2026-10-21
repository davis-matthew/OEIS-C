/*
 * A247395  The smallest numbers of every class in a classification of
 *          positive numbers (see comment).
 * https://oeis.org/A247395
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = (prime(n-1))^2 + 1   for n ≥ 3
 *          a(0) = 1, a(1) = 2, a(2) = 4
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Handle the small initial terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)4;

    /* For n ≥ 3: compute the (n‑1)th prime, square it, add 1 */
    uint32_t p = get_prime(n - 1);          /* nth prime (1‑based) */
    seq_t sq = (seq_t)p * (seq_t)p + (seq_t)1;
    return sq;
}
