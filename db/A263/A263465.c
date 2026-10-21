/*
 * A263465  Values of D for which the imaginary quadratic field Q[ sqrt(-D) ]
 *          is norm-Euclidean.
 * https://oeis.org/A263465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This sequence is finite: the imaginary quadratic fields with a norm-Euclidean
 * ring of integers are for D = 1, 2, 3, 7, 11.  No other positive D occur.
 * The function returns the corresponding value for n = 1..5 and 0 otherwise. */
seq_t a(idx_t n)
{
    /* table of the strictly increasing values */
    static const seq_t table[5] = {(seq_t)1, (seq_t)2, (seq_t)3, (seq_t)7, (seq_t)11};

    /* Guard against n == 0 (undefined index). */
    if (n == 0)
        return (seq_t)0;

    /* n-1 is safe because n >= 1. */
    idx_t idx = n - 1;

    if (idx < 5)
        return table[idx];

    /* For n > 5 the sequence has no defined term; return 0 as a sentinel. */
    return (seq_t)0;
}
