/*
 * A309881  Number of even parts appearing among the fourth largest parts of
 *          the partitions of n into 5 parts.
 * https://oeis.org/A309881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n^4 + 10*n^3 - 50*n^2 - 384*n + 2997 + 18*(n mod
 *            2)*n - 640*((2*n^2 + n) mod 3)) / 5760 )
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    /* Polynomial part of the numerator */
    seq_t part = n4 + 10 * n3 - 50 * n2 - 384 * nn + 2997;

    /* Even‑part contribution (only when n is odd) */
    seq_t parity = (nn & 1) * nn;          /* (n mod 2) * n */
    seq_t parity_term = 18 * parity;       /* 18 * (n mod 2) * n */

    /* Mod 3 correction term */
    seq_t mod3 = (2 * n2 + nn) % 3;        /* (2*n^2 + n) mod 3 */
    seq_t mod3_term = 640 * mod3;          /* 640 * ((2*n^2 + n) mod 3) */

    /* Full numerator */
    seq_t num = part + parity_term - mod3_term;

    return num / 5760;  /* floor division */
}
