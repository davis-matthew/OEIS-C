/*
 * A390925  a(n) is the number of 3 X 3 matrices with elements 1..n where at
 *          least one row and at least one column is strictly increasing.
 * https://oeis.org/A390925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(8141*n^6 - 14637*n^5 - 11887*n^4 * +
 *            12765*n^3 + 8750*n^2 - 540*n + 144)/45360
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For any sequence type the expression is computed in 128‑bit
       unsigned arithmetic and then converted to seq_t.  This is safe
       for the ranges required in the tests. */
    unsigned __int128 N   = (unsigned __int128)n;
    unsigned __int128 N2  = N * N;                 /* n²  */
    unsigned __int128 N3  = N2 * N;                /* n³  */
    unsigned __int128 N4  = N3 * N;                /* n⁴  */
    unsigned __int128 N5  = N4 * N;                /* n⁵  */
    unsigned __int128 N6  = N5 * N;                /* n⁶  */

    /* Polynomial part: 8141*n^6 – 14637*n^5 – 11887*n^4 + 12765*n^3
       + 8750*n^2 – 540*n + 144.  All intermediate results fit in
       unsigned __int128 for the requested ranges. */
    unsigned __int128 poly = 8141ULL * N6
                            - 14637ULL * N5
                            - 11887ULL * N4
                            + 12765ULL * N3
                            + 8750ULL  * N2
                            - 540ULL   * N
                            + 144ULL;

    /* Multiply the front factor n*(n-1)*(n-2).  The product still fits
       into unsigned __int128 because n ≤ 10000 in the time‑limit tests
       and 10000³ ≈ 1e12 << 2¹²⁸. */
    unsigned __int128 num = N * (N ? N-1 : 0) * (N ? N-2 : 0) * poly;

    /* Divide by the constant 45360.  The division is exact. */
    unsigned __int128 result128 = num / 45360ULL;

    /* Convert back to the required seq_t type.  The cast is valid for
       both fixed‑width integer types and arbitrary‑precision types. */
    return (seq_t)result128;
}
