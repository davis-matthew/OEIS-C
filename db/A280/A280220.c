/*
 * A280220  a(1) = 1, a(n+1) is the numerator of n-th partial fraction of the
 *          continued fraction [1; 4, 16, 64, ..., 4^n].
 * https://oeis.org/A280220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=5,
   a(n) = 4^(n-1)*a(n-1) + a(n-2)  for n>=3  */
seq_t a(idx_t n)
{
    if (n == 1U)
        return (seq_t)1;
    if (n == 2U)
        return (seq_t)5;

    seq_t a_prev2 = (seq_t)1; /* a(1) */
    seq_t a_prev1 = (seq_t)5; /* a(2) */
    seq_t pow4    = (seq_t)4; /* will be multiplied before each step */

    for (idx_t i = 3U; i <= n; ++i)
    {
        pow4 *= (seq_t)4;                           /* 4^(i-1)  */
        seq_t a_i = a_prev1 * pow4 + a_prev2;     /* recurrence */
        a_prev2 = a_prev1;
        a_prev1 = a_i;
    }
    return a_prev1; /* a(n) */
}
