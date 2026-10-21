/*
 * A292542  Number of 4-cycles in the n-Sierpinski tetrahedron graph.
 * https://oeis.org/A292542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 39*4^(n - 2) for n > 1; a(1) = 3; a(2) = 39
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n <= 0 we return 0 as a safe,
       though these values are never queried by the test harness. */
    if (n <= 0)
        return (seq_t)0;

    if (n == 1)
        return (seq_t)3;

    if (n == 2)
        return (seq_t)39;

    /* For n > 2 compute 39 * 4^(n-2) by repeated multiplication.  This is
       O(n) and fast enough for n up to about 10,000, the maximum tested
       value.  Using the simple multiplication operator <code>*</code> works
       both for built‑in unsigned integer types and for the arbitrary‑
       precision integer class used in the C++ build. */
    seq_t result = (seq_t)39;
    idx_t exp = n - 2;
    while (exp > 0)
    {
        result *= (seq_t)4;
        --exp;
    }
    return result;
}
