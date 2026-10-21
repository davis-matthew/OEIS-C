/*
 * A269707  Decimal expansion of x = 3*Sum_{n in E} 1/10^n where E is the set
 *          of numbers whose base-4 representation consists of only 0's and
 *          1's.
 * https://oeis.org/A269707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = 3 if (n-1) has only 0 or 1 digits in base‑4, otherwise
 *            a(n) = 0. The set E consists of integers whose base‑4
 *            representation contains no digit 2 or 3
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; return 0 for n == 0 to avoid
       underflow on unsigned subtraction. */
    if (n == 0)
        return (seq_t)0;

    idx_t m = n - 1;          /* position index starting from 0 */
    while (m > 0) {           /* test each base‑4 digit */
        if ((m & 3) > 1)      /* digit is 2 or 3 -> not in E */
            return (seq_t)0;
        m >>= 2;              /* move to next digit */
    }
    return (seq_t)3;          /* m == 0 corresponds to a member of E */
}
