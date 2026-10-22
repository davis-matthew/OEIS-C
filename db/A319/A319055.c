/*
 * A319055  Maximum product of an integer partition of n with relatively prime
 *          parts.
 * https://oeis.org/A319055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=6, a(6)=6, a(7)=12; For
 *            n≥8: if n%3==0 : a(n) = a(n-1)*4/3 else : a(n) = a(n-1)*3/2
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return 1;
    if (n == 3) return 2;
    if (n == 4) return 3;
    if (n == 5) return 6;
    if (n == 6) return 6;
    if (n == 7) return 12;

    seq_t val = 12;                 /* a(7) */
    for (idx_t i = 8; i <= n; ++i) {
        if ((i % 3) == 0) {           /* multiply by 4/3 */
            val = (val / 3) * 4;
        } else {                      /* multiply by 3/2 */
            val = (val / 2) * 3;
        }
    }
    return val;
}
