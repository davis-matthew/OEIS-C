/*
 * A375531  Lexicographically earliest sequence of positive integers a(1),
 *          a(2), a(3), ... such that for any n > 0, Sum_{k = 1..n} k!/a(k) <
 *          1.
 * https://oeis.org/A375531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A375531
 *
 *  Lexicographically earliest sequence of positive integers a(1), a(2), ...  
 *  such that for any n > 0, Σ_{k=1..n} k!/a(k) < 1.
 *
 *  Recurrence:  a(1) = 2
 *               a(n) = n! * Π_{k=1}^{n-1} a(k) + 1   (n ≥ 2)
 *
 *  This implementation computes a(n) by maintaining the factorial
 *  and the running product Π a(k) in arbitrary‑precision arithmetic.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* factorial and cumulative product of previous terms */
    seq_t factorial = 1;   /* n! */
    seq_t product   = 1;   /* Π_{k=1}^{n-1} a(k) */

    /* iterate from 1 up to n, building terms on the way */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* factorial *= k; cast k to seq_t to keep type consistent */
        factorial *= (seq_t)k;

        /* a(k) = factorial * product + 1 */
        seq_t ak = factorial * product + 1;

        if (k == n)
            return ak;   /* desired term reached */

        /* update product for next iteration */
        product *= ak;
    }

    /* n is guaranteed ≥ 1, so code never reaches here */
    return 0;
}
