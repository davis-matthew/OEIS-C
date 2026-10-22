/*
 * A337286  a(n) = Sum_{i=0..n} i^2*T(i)^2, where T(i) = A000073(i) is the
 *          i-th tribonacci number.
 * https://oeis.org/A337286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n} i^2 * T(i)^2, where T(i) is the Tribonacci
 *            number A000073
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;

    // Tribonacci terms: T0 = 0, T1 = 0, T2 = 1
    seq_t trib0 = 0, trib1 = 0, trib2 = 1;
    seq_t currTrib;

    for (idx_t i = 0; i <= n; ++i)
    {
        // Get current Tribonacci number
        if (i == 0)
            currTrib = trib0;              /* 0 */
        else if (i == 1)
            currTrib = trib1;              /* 0 */
        else if (i == 2)
            currTrib = trib2;              /* 1 */
        else
        {
            currTrib = trib0 + trib1 + trib2;  /* T(i) = T(i-3)+T(i-2)+T(i-1) */
            trib0 = trib1;
            trib1 = trib2;
            trib2 = currTrib;
        }

        // term = i^2 * T(i)^2
        seq_t idx2   = (seq_t)i * (seq_t)i;
        seq_t term   = idx2 * currTrib;
        term         = term * currTrib;   /* term = i^2 * T(i)^2 */

        result += term;
    }

    return result;
}
