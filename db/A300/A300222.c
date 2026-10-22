/*
 * A300222  In ternary (base-3) representation of n, replace 1's with 0's.
 * https://oeis.org/A300222
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{i | digit_i(n)=2} 2*3^i (in base‑3 representation,
 *            replace all 1's by 0's and evaluate)
 * Range:     u8 n <= 485, u16 n <= 9856, u32 n <= 9856, u64 n <= 9856,
 *            u128 n <= 9856, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;           /* accumulate answer */
    seq_t pow3   = 1;           /* current 3^position (seq_t to avoid overflow) */

    while (n > 0)
    {
        idx_t digit = n % 3;    /* current ternary digit */
        n /= 3;

        if (digit == 2)
            result += (seq_t)2 * pow3;   /* keep digit 2 */

        pow3 *= (seq_t)3;      /* advance to next higher position */
    }

    return result;
}
