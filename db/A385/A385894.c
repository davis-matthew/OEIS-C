/*
 * A385894  a(n) = n^5/5 + n^3/3 + 7*n/15.
 * https://oeis.org/A385894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^5 + 5*n^3 + 7*n) / 15 (OEIS A385894)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 116, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /*  Use a 128‑bit unsigned integer for the intermediate calculations
     *  so that the 64‑bit build never overflows while keeping the
     *  operations compliant with the allowed operators.  */
    unsigned __int128 N  = (unsigned __int128)n;   /* convert index */
    unsigned __int128 N2 = N * N;                  /* n²   */
    unsigned __int128 N3 = N2 * N;                 /* n³   */
    unsigned __int128 N5 = N3 * N2;                /* n⁵   */
    unsigned __int128 num = 3 * N5 + 5 * N3 + 7 * N;
    unsigned __int128 res = num / 15;              /* exact division */
    return (seq_t)res;                             /* cast back to seq_t */
}
