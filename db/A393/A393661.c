/*
 * A393661  A sequence appearing in the 3-Bratteli diagram,
 *          a(n)=3^(n-3)*(16*n-17), n>=3.
 * https://oeis.org/A393661
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^(n-3)*(16*n-17)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 3 the offset is irrelevant; return 0 as a safe default. */
    if (n < 3)
        return (seq_t)0;

    /* Compute 3^(n-3) by repeated multiplication in seq_t */
    seq_t pow3 = (seq_t)1;
    idx_t exp = n - 3;
    for (idx_t i = 0; i < exp; ++i)
        pow3 *= (seq_t)3;

    /* Linear factor (16*n-17) fits in idx_t for the n used in tests,
       cast it to seq_t for the final multiplication. */
    idx_t coeff = 16 * (idx_t)n - (idx_t)17;

    return pow3 * (seq_t)coeff;
}
