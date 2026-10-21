/*
 * A254658  Numbers of words on alphabet {0,1,...,7} with no subwords ii,
 *          where i is from {0,1,2,3}.
 * https://oeis.org/A254658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 4*a(n-2), a(0) = 1, a(1) = 8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)8;
    seq_t an = a1;              /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        an = 7 * a1 + 4 * a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
