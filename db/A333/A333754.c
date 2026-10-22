/*
 * A333754  Sum of the areas of all r X s rectangles such that r < s, r + s =
 *          2n and (s - r) | (s * r).
 * https://oeis.org/A333754
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{i=1}^{n-1} i*(2n-i) * chi( (i*(2n-i)) / (2n-2i) )
 *            where chi(x) = 1 if x is an integer, 0 otherwise. This is
 *            equivalent to adding the areas i*(2n-i) for all i with
 *            (i*(2n-i)) divisible by 2(n-i)
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n <= 1) return sum;          /* a(1) = 0, a(0) not used */

    seq_t twoN = (seq_t)n + (seq_t)n; /* 2*n in seq_t to avoid overflow */

    for (idx_t i = 1; i < n; ++i)
    {
        seq_t r = (seq_t)i;
        seq_t s = twoN - (seq_t)i;          /* 2n - i */
        idx_t diff = n - i;                 /* n - i, safe because i < n */
        seq_t d = (seq_t)2 * (seq_t)diff;   /* 2(n-i) */

        seq_t area = r * s;                 /* r*s */
        if (area % d == 0)                  /* divisibility check */
            sum += area;
    }
    return sum;
}
