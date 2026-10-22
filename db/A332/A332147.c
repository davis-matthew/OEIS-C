/*
 * A332147  a(n) = 4*(10^(2*n+1)-1)/9 + 3*10^n.
 * https://oeis.org/A332147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*(10^(2*n+1)-1)/9 + 3*10^n } /* Computes a(n) for any
 *            non‑negative n. The routine is intentionally written to avoid
 *            the intermediate 10^(2*n+1) growth that would overflow uint64_t.
 *            It builds the required repunit and powers of ten by repeated
 *            multiplication, which works unchanged for the a...
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Build repunit of length 2*n+1 : 111...1 */
    seq_t rep = 0;
    for (idx_t i = 0; i < (2*n + 1); ++i)
        rep = rep * 10 + 1;

    /* term1 = 4 * rep */
    seq_t term1 = rep * 4;

    /* Build 10^n */
    seq_t ten = 1;
    for (idx_t i = 0; i < n; ++i)
        ten *= 10;

    /* term2 = 3 * 10^n */
    seq_t term2 = ten * 3;

    return term1 + term2;
}
