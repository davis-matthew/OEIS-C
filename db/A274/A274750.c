/*
 * A274750  Number of 3 X n 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (0,-1) or (-1,0) and new values introduced in order
 *          0..2.
 * https://oeis.org/A274750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 2*a(n-3) - 2*a(n-4) (n≥5) Initial
 *            values: a(1)=2, a(2)=9, a(3)=34, a(4)=133
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t init[4] = { (seq_t)2, (seq_t)9, (seq_t)34, (seq_t)133 };

    if (n == 0) return (seq_t)0;          /* offset convention, unused values */
    if (n <= 4) return init[n - 1];

    seq_t prev4 = init[0];   /* a(1) */
    seq_t prev3 = init[1];   /* a(2) */
    seq_t prev2 = init[2];   /* a(3) */
    seq_t prev1 = init[3];   /* a(4) */
    seq_t cur;

    for (idx_t i = 5; i <= n; ++i) {
        cur = (seq_t)2 * prev1 + (seq_t)7 * prev2 +
              (seq_t)2 * prev3 - (seq_t)2 * prev4;
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
