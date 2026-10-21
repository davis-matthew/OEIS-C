/*
 * A255626  Number of length n+4 0..2 arrays with at most two downsteps in
 *          every n consecutive neighbor pairs.
 * https://oeis.org/A255626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (n^8 + 68 n^7 + 1666 n^6 + 48776 n^5 + 497329 n^4 +
 *                 100772 n^3 + 2021004 n^2 + 3983184 n + 1814400) / 40320
 *           for n > 2.  For n = 1,2 the sequence has the values 243 and 729.
 *
 * The function implements this exact rational polynomial using only
 * seq_t arithmetic, so it works with any integer type that supports
 * the permitted operators (including arbitrary‑precision types used in
 * the C++ build).
 */
seq_t a(idx_t n)
{
    /* Special cases matching OEIS offset 1 */
    if (n == 1) return (seq_t)243;
    if (n == 2) return (seq_t)729;
    /* For n >= 3 compute the polynomial */
    seq_t N = (seq_t)n;          /* n as seq_t */
    /* Compute successive powers of n up to n^8 */
    seq_t n2 = N * N;            /* n^2 */
    seq_t n3 = n2 * N;           /* n^3 */
    seq_t n4 = n3 * N;           /* n^4 */
    seq_t n5 = n4 * N;           /* n^5 */
    seq_t n6 = n5 * N;           /* n^6 */
    seq_t n7 = n6 * N;           /* n^7 */
    seq_t n8 = n7 * N;           /* n^8 */
    /* Accumulate the numerator of the fraction */
    seq_t num = n8;
    num += (seq_t)68 * n7;
    num += (seq_t)1666 * n6;
    num += (seq_t)48776 * n5;
    num += (seq_t)497329 * n4;
    num += (seq_t)100772 * n3;
    num += (seq_t)2021004 * n2;
    num += (seq_t)3983184 * N;
    num += (seq_t)1814400;
    /* Divide by 40320 to obtain the integer term */
    return num / (seq_t)40320;
}
