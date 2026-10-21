/*
 * A250158  Number of length 4+6 0..n arrays with every seven consecutive
 *          terms having the maximum of some two terms equal to the minimum of
 *          the remaining five terms.
 * https://oeis.org/A250158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5/21)*n^9 + (127/21)*n^8 + (251/7)*n^7 + (911/10)*n^6 +
 *            (684/5)*n^5 + (325/2)*n^4 + (3047/21)*n^3 + (6967/105)*n^2 +
 *            (946/105)*n + 1
 * Range:     u8 none, u16 n <= 2, u32 n <= 11, u64 n <= 159, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Use a 128‑bit intermediate to avoid overflow when seq_t is 64 bits. */
    unsigned __int128 val = 50;
    val = val * (unsigned __int128)nn + 1270;
    val = val * (unsigned __int128)nn + 7530;
    val = val * (unsigned __int128)nn + 19131;
    val = val * (unsigned __int128)nn + 28728;
    val = val * (unsigned __int128)nn + 34125;
    val = val * (unsigned __int128)nn + 30470;
    val = val * (unsigned __int128)nn + 13934;
    val = val * (unsigned __int128)nn + 1892;
    val = val * (unsigned __int128)nn + 210;
    return (seq_t)(val / 210);
}
