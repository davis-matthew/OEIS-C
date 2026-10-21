/*
 * A258886  Number of (n+2)X(n+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum less than the antidiagonal sum or central row sum less than
 *          the central column sum.
 * https://oeis.org/A258886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 242 (n=1), 2304 (n=2), 6072 (n=3), for n≥4: if n even:
 *            let m = (n-4)/2; a(n) = [8*m*m + 52*m + 85]² if n odd : let m =
 *            (n-5)/2; a(n) = [8*m*m + 60*m + 112]²
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)242;
    if (n == 2) return (seq_t)2304;
    if (n == 3) return (seq_t)6072;

    seq_t k;
    if ((n & 1) == 0) {          /* n even */
        idx_t m = (n - 4) / 2;
        seq_t mm = (seq_t)m;
        k = (seq_t)8 * mm * mm + (seq_t)52 * mm + (seq_t)85;
    } else {                     /* n odd */
        idx_t m = (n - 5) / 2;
        seq_t mm = (seq_t)m;
        k = (seq_t)8 * mm * mm + (seq_t)60 * mm + (seq_t)112;
    }
    return k * k;
}
