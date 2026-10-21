/*
 * A245561  a(n) = 5^n - ( (sqrt(5)*phi)^n + (sqrt(5)/phi)^n ) + 1, where phi
 *          = golden ratio A001622.
 * https://oeis.org/A245561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^n - A020876(n) + 1 A020876: B_0 = 2, B_1 = 5, B_n =
 *            5*B_{n-1} - 5*B_{n-2} for n >= 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for any non‑negative n.  The function works for all
   supported seq_t types, including arbitrary‑precision big integers
   (C++ implementation) and fixed‑width unsigned integers. */
seq_t a(idx_t n)
{
    /* Handle the special case n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* 5^n accumulator, start with 5^0 = 1 */
    seq_t pow5 = (seq_t)1;

    /* Recurrence for B_n: B_0 = 2, B_1 = 5 */
    seq_t B_prev2 = (seq_t)2;        /* B_{i-2}  */
    seq_t B_prev1 = (seq_t)5;        /* B_{i-1}  */
    seq_t B_curr   = (seq_t)0;       /* B_i      */

    /* Loop to compute 5^n and B_n up to n */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Compute next power of 5: 5^i */
        pow5 = pow5 * (seq_t)5;

        if (i == 1)
        {
            /* B_1 is already known; no update needed */
            B_curr = B_prev1;
        }
        else
        {
            /* B_i = 5*B_{i-1} - 5*B_{i-2}  */
            B_curr = (seq_t)5 * B_prev1 - (seq_t)5 * B_prev2;
            /* Shift for next iteration */
            B_prev2 = B_prev1;
            B_prev1 = B_curr;
        }
    }

    /* a(n) = 5^n - B_n + 1 */
    return pow5 - B_curr + (seq_t)1;
}
