/*
 * A360942  a(n) is the least k such that k*n is a happy number (A007770).
 * https://oeis.org/A360942
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least k such that k*n is a happy number
 * Range:     u8 n <= 8, u16 n <= 2952, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes the square‑sum of the decimal digits of x. */
static inline seq_t sum_square_digits(seq_t x)
{
    seq_t sum = 0;
    while (x > 0)
    {
        seq_t d   = x % 10;
        sum += d * d;
        x   /= 10;
    }
    return sum;
}

/* Returns true iff x is a happy number (decimal). */
static inline bool is_happy(seq_t x)
{
    /* All happy numbers eventually reach 1, while all unhappy
       numbers cycle through 4→16→37→…→4.  Therefore the loop
       terminates with one of the two values. */
    while (x != 1 && x != 4)
        x = sum_square_digits(x);
    return x == 1;
}

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for n=0, but avoid UB */
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t k = 1;

    for (;; ++k)
    {
        seq_t product = n_seq * k;
        if (is_happy(product))
            return k;
    }
}
