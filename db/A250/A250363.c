/*
 * A250363  Number of length 5 arrays x(i), i=1..5 with x(i) in i..i+n and no
 *          value appearing more than 3 times.
 * https://oeis.org/A250363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 5*n^4 + 10*n^3 + 5*n^2 + 17*n + 2 for n > 2 a(1) =
 *            32, a(2) = 243
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function respects the offset of 1 in OEIS. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)32;
    if (n == 2)
        return (seq_t)243;

    /* Convert n to the sequence type to avoid mixing idx_t and seq_t. */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n. */
    seq_t n2 = nn * nn;       /* n^2 */
    seq_t n3 = n2 * nn;       /* n^3 */
    seq_t n4 = n3 * nn;       /* n^4 */
    seq_t n5 = n4 * nn;       /* n^5 */

    /* Evaluate the polynomial. */
    seq_t res = n5
                + ((seq_t)5) * n4
                + ((seq_t)10) * n3
                + ((seq_t)5) * n2
                + ((seq_t)17) * nn
                + ((seq_t)2);

    return res;
}
