/*
 * A252536  Number of (4+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 2 3 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 2 3 6 or 7.
 * https://oeis.org/A252536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 535 if n=1 426 if n=2 610 if n=3 790 if n=4 1592 if n=5
 *            4*a(n-3) if n>5
 * Range:     u8 none, u16 n <= 13, u32 n <= 37, u64 n <= 85, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static const seq_t base[6] = { (seq_t)0, (seq_t)535, (seq_t)426,
                               (seq_t)610, (seq_t)790, (seq_t)1592 };

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 5) return base[n];

    seq_t result = 1;
    idx_t m = n;
    while (m > 5) {
        result *= (seq_t)4;
        m -= 3;
    }
    return result * base[m];
}
