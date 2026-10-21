/*
 * A291237  p-INVERT of (0,1,0,1,0,1,...), where p(S) = (1 - S)(1 - 2 S)(1 - 4
 *          S).
 * https://oeis.org/A291237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) - 11*a(n-2) - 6*a(n-3) + 11*a(n-4) + 7*a(n-5) +
 *            a(n-6) (a(0)=7, a(1)=35, a(2)=162, a(3)=721, a(4)=3139,
 *            a(5)=13504, a(6)=57707)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial values */
    static const seq_t init[7] = { 7, 35, 162, 721, 3139, 13504, 57707 };
    if (n < 7)
        return init[n];

    /* Shift register holding a(n-1) … a(n-7) */
    seq_t reg[7] = { init[6], init[5], init[4], init[3], init[2], init[1], init[0] };

    for (idx_t i = 7; i <= n; ++i)
    {
        seq_t val = 7 * reg[0] - 11 * reg[1] - 6 * reg[2]
                    + 11 * reg[3] + 7 * reg[4] + reg[5];

        /* shift right and insert new value at front */
        for (int j = 6; j > 0; --j)
            reg[j] = reg[j - 1];
        reg[0] = val;
    }
    return reg[0];
}
