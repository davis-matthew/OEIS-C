/*
 * A334909  Area/6 of primitive Pythagorean triangles given in A334638 as
 *          triples.
 * https://oeis.org/A334909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-1) - 280*a(n-2) + 960*a(n-3) - 1024*a(n-4) Initial
 *            values: a(0)=1, a(1)=35, a(2)=770, a(3)=14260
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 35;
    if (n == 2) return 770;
    if (n == 3) return 14260;

    /* use a sliding window of the last four terms */
    seq_t a0 = 1;          /* a(n-4) */
    seq_t a1 = 35;         /* a(n-3) */
    seq_t a2 = 770;        /* a(n-2) */
    seq_t a3 = 14260;      /* a(n-1) */
    seq_t ai;

    for (idx_t i = 4; i <= n; ++i) {
        ai = 30 * a3 - 280 * a2 + 960 * a1 - 1024 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3;
}
