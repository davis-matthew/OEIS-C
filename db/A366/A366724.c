/*
 * A366724  First index k where A366574(k) = n.
 * https://oeis.org/A366724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 1                     for n = 1
 *            a(n) = (n-1)*(n^3 + 3*n^2 - 10*n + 72) / 24   for n > 1
 * (Conjectured formula from OEIS A366724)
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    seq_t sn   = (seq_t)n;          /* n as seq_t */
    seq_t n1   = sn - (seq_t)1;      /* n-1 */
    seq_t n2   = sn * sn;            /* n^2 */
    seq_t n3   = n2 * sn;            /* n^3 */

    seq_t tmp = n3 + (seq_t)3 * n2;   /* n^3 + 3*n^2 */
    tmp = tmp - (seq_t)10 * sn;       /* - 10*n */
    tmp = tmp + (seq_t)72;            /* + 72 */

    seq_t num = tmp * n1;            /* multiply by (n-1) */
    seq_t res = num / (seq_t)24;     /* exact division by 24 */

    return res;
}
