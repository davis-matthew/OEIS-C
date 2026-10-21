/*
 * A299913  a(n) = a(n-1) + 2*a(n-2) if n even, or 3*a(n-1) + 4*a(n-2) if n
 *          odd, starting with 0, 1.
 * https://oeis.org/A299913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) if n even, or a(n) = 3*a(n-1) +
 *            4*a(n-2) if n odd, with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0; /* a(0) */
    seq_t prev1 = (seq_t)1; /* a(1) */
    seq_t cur   = prev1;

    for (idx_t i = 2; i <= n; ++i) {
        if (i & 1) { /* odd */
            cur = prev1 * (seq_t)3 + prev2 * (seq_t)4;
        } else {    /* even */
            cur = prev1 + prev2 * (seq_t)2;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
