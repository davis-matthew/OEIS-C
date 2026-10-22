/*
 * A309879  Number of odd parts appearing among the fourth largest parts of
 *          the partitions of n into 5 parts.
 * https://oeis.org/A309879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^4+10*n^3+70*n^2+144*n+1917)/5760 + (n mod
 *            2)*9*n/320 + ((2*n^2+n) mod 3)*2/9)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          Equivalent integer computation: a(n) = (A + 18*B + 640*C)/5760, where 
 *          A = n^4 + 10*n^3 + 70*n^2 + 144*n + 1917,
 *          B = (n mod 2) * 9 * n,
 *          C = ((2*n^2 + n) mod 3) * 2. */
seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    seq_t A = n4 + 10 * n3 + 70 * n2 + 144 * nn + 1917;

    seq_t B = (nn & 1) * (9 * nn);

    seq_t C = ((2 * n2 + nn) % 3) * 2;

    seq_t numer = A + 18 * B + 640 * C;
    seq_t result = numer / 5760;

    return result;
}
