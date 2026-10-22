/*
 * A333861  The sum of the Hamming weights of the elements of the Collatz
 *          orbit of n.
 * https://oeis.org/A333861
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look
 * Formula:   a(n) = popcount(n) + a(next(n)), with a(1) = 1 where next(n) =
 *            n/2 if n is even, otherwise 3*n + 1. The function computes the
 *            sum of Hamming weights over the Collatz orbit of n, including n
 *            itself and 1
 * Range:     u8 n <= 26, u16 n <= 5344, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute Hamming weight of a seq_t value. */
static inline seq_t popcount_seq(seq_t x)
{
    seq_t c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

/* Compute the next term in the Collatz sequence. */
static inline seq_t collatz_next(seq_t x)
{
    return (x & 1) ? (x * 3 + 1) : (x >> 1);
}

seq_t a(idx_t n)
{
    seq_t x   = (seq_t)n;
    seq_t sum = 0;

    while (1) {
        sum += popcount_seq(x);
        if (x == 1) break;
        x = collatz_next(x);
    }
    return sum;
}
