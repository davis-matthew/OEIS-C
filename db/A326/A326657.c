/*
 * A326657  a(n) = 4*floor(n/2) + ceiling((n-1)^2/2).
 * https://oeis.org/A326657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*floor(n/2) + ceiling((n-1)^2/2)
 * Range:     u8 n <= 16, u16 n <= 360, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 4 * floor(n/2) */
    idx_t half = n / 2;
    seq_t part1 = (seq_t)half * 4;

    /* ceil((n-1)^2 / 2) */
    seq_t ceil_val;
    if (n == 0) {
        /* (n-1) = -1 → (-1)^2 = 1 */
        seq_t sq = (seq_t)1;
        ceil_val = (sq + 1) / 2;   /* (1+1)/2 = 1 */
    } else {
        idx_t m = n - 1;           /* (n-1) as unsigned */
        seq_t sq   = (seq_t)m * (seq_t)m;
        ceil_val = (sq + 1) / 2;
    }

    return part1 + ceil_val;
}
