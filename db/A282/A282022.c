/*
 * A282022  Start with 1; multiply alternately by 3 and 4.
 * https://oeis.org/A282022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-2) for n>1; a(0)=1; a(1)=3
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    /* For even indices: a(n) = 12^(n/2)
       For odd  indices: a(n) = 3 * 12^((n-1)/2) */
    idx_t exp = n / 2;
    seq_t val = (n & 1) ? (seq_t)3 : (seq_t)1;  /* odd => 3, even => 1 */

    /* Multiply by 12 exp times */
    for (idx_t i = 0; i < exp; ++i)
        val *= (seq_t)12;

    return val;
}
