/*
 * A307116  A special version of Pascal's triangle where only Fibonacci
 *          numbers are permitted.
 * https://oeis.org/A307116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the n‑th element (0‑indexed) of the special Pascal’s
 *            triangle where each entry is the sum of the two numbers above it
 *            if that sum is a Fibonacci number; otherwise the entry is 1. The
 *            triangle is generated row by row and left‑to‑right. a(n) is
 *            obtained by flattening the triangle row‑wise
 * Range:     u8 n <= 9869, u16 n <= 9869, u32 n <= 9869, u64 n <= 9869,
 *            u128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Test whether x is a Fibonacci number.
   Works for any unsigned integer type seq_t (including
   arbitrary‑precision types). */
static bool is_fibonacci(seq_t x)
{
    if (x == (seq_t)1) return true;
    seq_t a = (seq_t)1, b = (seq_t)1;
    while (b < x)
    {
        seq_t c = a + b;
        a = b;
        b = c;
    }
    return b == x;
}

/* Return the nth term of the sequence.  n is 0‑based. */
seq_t a(idx_t n)
{
    /* Find the row r and column k (0‑based) of the n‑th entry. */
    idx_t r = 0;
    idx_t cum = 0;               /* total entries before row r */
    while (cum + r + 1 <= n)
    {
        cum += r + 1;
        r++;
    }
    idx_t k = n - cum;           /* column within row r */

    /* The largest required row is at most about 140 for n ≤ 10000,
       so static arrays of size 200 are sufficient for any n in the
       allowed range.  They also work when seq_t is a class type,
       because the array is allocated on the stack. */
    seq_t prev[200];
    seq_t cur[200];

    /* Row 0 */
    prev[0] = (seq_t)1;
    if (r == 0) return prev[0];

    /* Build rows up to r. */
    for (idx_t row = 1; row <= r; ++row)
    {
        cur[0] = (seq_t)1;
        cur[row] = (seq_t)1;

        for (idx_t j = 1; j < row; ++j)
        {
            seq_t s = prev[j - 1] + prev[j];
            cur[j] = is_fibonacci(s) ? s : (seq_t)1;
        }

        if (row == r) /* we have the desired value */
            return cur[k];

        /* copy cur to prev for the next iteration */
        for (idx_t j = 0; j <= row; ++j)
            prev[j] = cur[j];
    }

    /* Should never reach here */
    return (seq_t)0;
}
