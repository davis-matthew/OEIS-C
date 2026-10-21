/*
 * A295827  a(n) = least odd k > 1 such that n and n*k have the same Hamming
 *          weight, or -1 if no such k exists.
 * https://oeis.org/A295827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 32, i32 n <= 260, i64 n <= 2052,
 *            i128 n <= 4112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = least odd k > 1 such that n and n*k have the same Hamming weight.
   For n being a power of two a(n) = -1.  The recursion a(2*n) = a(n) is used
   to reduce even indices.  The function is computed by a direct search for k. */

#include <stdint.h>

/* Count the number of set bits in a seq_t value. */
static inline unsigned int hamming_weight(seq_t x)
{
    unsigned int cnt = 0;
    while (x)
    {
        cnt += (unsigned int)(x & 1);
        x >>= 1;
    }
    return cnt;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* Offset starts at 1 */
    if (n == 0u) return -1;

    /* a(2^k) = -1 */
    if ((n & (n - 1)) == 0) return -1;

    /* Reduce even indices using a(2*n) = a(n) */
    if ((n & 1) == 0) return a(n >> 1);

    /* Now n is odd and not a power of two */
    unsigned int wt = hamming_weight((seq_t)n);

    for (seq_t k = 3; /* odd k > 1 */ ; k += 2)
    {
        seq_t prod = (seq_t)n * k;
        if (hamming_weight(prod) == wt)
            return k;
    }
}
