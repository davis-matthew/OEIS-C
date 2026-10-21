/*
 * A245766  a(n) = 2*(n^2 + 1) - n*(1 + (-1)^n).
 * https://oeis.org/A245766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^2 + 1) - n*(1 + (-1)^n)
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;            /* convert index to seq_t */
    if ((n & 1) == 0) {              /* even n */
        seq_t val = n_s * n_s;       /* n^2 */
        val -= n_s;                  /* n^2 - n */
        val += 1;                    /* n^2 - n + 1 */
        val *= 2;                    /* 2*(n^2 - n + 1) */
        return val;
    } else {                         /* odd n */
        seq_t val = n_s * n_s;       /* n^2 */
        val += 1;                    /* n^2 + 1 */
        val *= 2;                    /* 2*(n^2 + 1) */
        return val;
    }
}
