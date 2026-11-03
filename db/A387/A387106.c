/*
 * A387106  Number of iterations of seven segments count x -> A074458(x) to go
 *          from n to a fixed point.
 * https://oeis.org/A387106
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 87 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of seven‑segment counts of the decimal digits of x. */
static seq_t seg_sum(seq_t x)
{
    if (x == 0)
        return (seq_t)6;                     /* 0 uses 6 segments */

    static const unsigned seg[10] = {
        6, /* 0 */
        2, /* 1 */
        5, /* 2 */
        5, /* 3 */
        4, /* 4 */
        5, /* 5 */
        6, /* 6 */
        4, /* 7 */
        7, /* 8 */
        5  /* 9 */
    };

    seq_t sum = 0;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        sum += (seq_t)seg[d];
        x /= 10;
    }
    return sum;
}
/* Compute the number of iterations of the seven‑segment digit
   counter A074458 needed to reach a fixed point (4, 5 or 6) for a
   given non‑negative integer n.  The mapping used by A074458
   follows the segment counts
   {0:6, 1:2, 2:5, 3:5, 4:4, 5:5, 6:6, 7:4, 8:7, 9:5}. */

seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    if (cur == 4 || cur == 5 || cur == 6)
        return 0; /* already a fixed point */

    seq_t steps = 0;
    while (cur != 4 && cur != 5 && cur != 6) {
        cur = seg_sum(cur);
        ++steps;
    }
    return steps;
}
