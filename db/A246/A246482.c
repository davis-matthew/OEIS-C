/*
 * A246482  Number of length 3+3 0..n arrays with no pair in any consecutive
 *          four terms totalling exactly n.
 * https://oeis.org/A246482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is a polynomial of degree 6 a(n) = n^6 - 6 n^5 + 21 n^4 -
 *            40 n^3 + 43 n^2 - 20 n for n even a(n) = n^6 - 6 n^5 + 21 n^4 -
 *            30 n^3 + 5 n^2 + 37 n - 26 for n odd (as conjectured by Colin
 *            Barker, 11‑term recurrence verified up to a(24). This
 *            implementation evaluates the polynomial exactly in seq...
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 41, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nt = (seq_t)n;
    seq_t n2 = nt * nt;               /* n^2 */
    seq_t n3 = n2 * nt;               /* n^3 */
    seq_t n4 = n3 * nt;               /* n^4 */
    seq_t n5 = n4 * nt;               /* n^5 */
    seq_t n6 = n5 * nt;               /* n^6 */

    seq_t res;
    if ((n & 1) == 0) {               /* n even */
        res = n6;
        res -= 6 * n5;
        res += 21 * n4;
        res -= 40 * n3;
        res += 43 * n2;
        res -= 20 * nt;
    } else {                          /* n odd */
        res = n6;
        res -= 6 * n5;
        res += 21 * n4;
        res -= 30 * n3;
        res += 5 * n2;
        res += 37 * nt;
        res -= 26;
    }
    return res;
}
