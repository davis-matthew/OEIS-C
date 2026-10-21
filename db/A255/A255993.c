/*
 * A255993  Number of length n+2 0..1 arrays with at most one downstep in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^3 + 6n^2 + 23n + 18)/6 (OEIS A255993)
 * Range:     u8 n <= 3, u16 n <= 38, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to the sequence type to avoid overflow from idx_t */
    seq_t ns = (seq_t)n;
    seq_t n2 = ns * ns;            /* n^2 */
    seq_t n3 = n2 * ns;            /* n^3 */
    seq_t numer = n3 + (seq_t)6 * n2 + (seq_t)23 * ns + (seq_t)18;
    return numer / (seq_t)6;
}
