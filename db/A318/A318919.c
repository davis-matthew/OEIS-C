/*
 * A318919  Define b(0)=0, b(1)[1]=1, b(1)[2]=1; and for n>=2, b(n)[1] = total
 *          number of digits in b(n-1), and b(n)[2] = total number of digits
 *          in b(0),...,b(n-1); a(n) = b(n)[2].
 * https://oeis.org/A318919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is defined by * * b(0) = 0 * b(1) = [1, 1] * For n ≥ 2: *
 *            b(n)[1] = total number of digits in b(n−1) * b(n)[2] = total
 *            number of digits in b(0), … , b(n−1) * a(n) = b(n)[2] * *
 *            Recurrence (after eliminating the explicit b‑vector): * * Let
 *            D(n) = total number of digits in b(n) (D(1) = 2), * a...
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   digit_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_len.h"

/* Computes the n‑th term of A318919.  */
seq_t a(idx_t n)
{
    /* Undefined for n < 1; return 0 for safety. */
    if (n == 0) return 0;

    /* Base values for n = 1. */
    seq_t a_val = 1;          /* a(1) */
    seq_t d     = 2;          /* D(1) = digits(1) + digits(1) */

    /* Iterate for n >= 2. */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* a(n) = a(n−1) + D(n−1) */
        a_val += d;

        /* D(n) = digits(D(n−1)) + digits(a(n)) */
        int d_digits  = digit_len(d);
        int a_digits  = digit_len(a_val);
        seq_t new_d = (seq_t)d_digits + (seq_t)a_digits;

        d = new_d;
    }

    return a_val;
}
