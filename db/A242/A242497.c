/*
 * A242497  Sides of (Heronian) triangles where sides are consecutive integers
 *          and area is an integer.
 * https://oeis.org/A242497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*x_k - 1 + ((n-1)%3) where x_1 = 2, x_2 = 7 and for k ≥
 *            3 x_k = 4*x_{k-1} - x_{k-2}
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 48, u64 n <= 99, u128 n <= 201,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n=0, but guard anyway */

    /* group index k: 1 for n=1..3, 2 for n=4..6, etc. */
    idx_t k = (n + 2u) / 3u;

    seq_t x;   /* x_k */

    if (k == 1) {
        x = 2;
    } else if (k == 2) {
        x = 7;
    } else {
        seq_t x_prev = 2;
        seq_t x_cur  = 7;
        for (idx_t i = 3; i <= k; ++i) {
            seq_t x_next = 4 * x_cur - x_prev;
            x_prev = x_cur;
            x_cur  = x_next;
        }
        x = x_cur;
    }

    seq_t inc = (seq_t)((n - 1u) % 3u);
    return 2 * x - 1 + inc;
}
