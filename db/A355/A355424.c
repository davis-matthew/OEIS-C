/*
 * A355424  Positive integers m such that the real quadratic fields of the
 *          form Q(sqrt(m^2+4)) have class number 1.
 * https://oeis.org/A355424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = 0 for n > 6; 4·n − 7 if n ∈ {5,6}; else a(n) = 1 + 2·(n −
 *            1)
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two special indices */
    if (n == 5 || n == 6)
        return (seq_t)4 * (seq_t)n - (seq_t)7;

    /* for n > 6 the sequence has no further terms */
    if (n > 6)
        return (seq_t)0;

    /* general case – valid for n = 1,2,3,4 */
    return (seq_t)1 + (seq_t)2 * ((seq_t)n - (seq_t)1);
}
