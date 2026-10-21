/*
 * A254659  Numbers of words on alphabet {0,1,...,8} with no subwords ii,
 *          where i is from {0,1,2,3}.
 * https://oeis.org/A254659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 9, a(n) = 8*a(n‑1) + 5*a(n‑2) (n > 1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)9;

    seq_t a0 = (seq_t)1;    /* a(0) */
    seq_t a1 = (seq_t)9;    /* a(1) */
    seq_t a  = a1;          /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        a = (seq_t)8 * a1 + (seq_t)5 * a0;
        a0 = a1;
        a1 = a;
    }

    return a1;
}
