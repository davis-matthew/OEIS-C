/*
 * A262577  Concatenation of the numbers from 1 to n but omitting 7.
 * https://oeis.org/A262577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n‑1) * 10^digit_count(e(n)) + e(n) * where e(n) = n ,
 *            for n ≤ 6 * e(n) = n+1 , for n > 6 * and a(1) = 1. * The
 *            function recomputes a(n) from scratch; it is valid for any * n ≥
 *            1 and for any build of seq_t and idx_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        idx_t val = (i <= 6) ? i : i + 1;   /* integer to attach */

        /* Compute 10^d where d = number of decimal digits of val. */
        seq_t pow10 = 1;
        idx_t tmp = val;
        while (tmp > 0)
        {
            pow10 *= 10;
            tmp /= 10;
        }

        /* Append the new integer to the running concatenation. */
        result = result * pow10 + (seq_t)val;
    }
    return result;
}
