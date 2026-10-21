/*
 * A250122  Coordination sequence for planar net 3.12.12.
 * https://oeis.org/A250122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   piecewise as given in OEIS entry * a(0) = 1 * a(4k) = 10k - 2 (k
 *            > 0) * a(4k+1) = 9k + 3 * a(4k+2) = 8k + 6 (k > 0, k=0 gives
 *            a(2)=4) * a(4k+3) = 9k + 6
 * Range:     u8 n <= 103, u16 n <= 1021, u32 n <= 1021, u64 n <= 1021,
 *            u128 n <= 1021, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    idx_t k = n / 4;
    idx_t r = n % 4;
    switch (r) {
        case 0: /* 4k, k > 0 */
            return ((seq_t)10 * (seq_t)k) - (seq_t)2;
        case 1: /* 4k+1 */
            return ((seq_t)9 * (seq_t)k) + (seq_t)3;
        case 2: /* 4k+2, k > 0; k=0 handled separately */
            if (n == 2) return (seq_t)4;
            return ((seq_t)8 * (seq_t)k) + (seq_t)6;
        case 3: /* 4k+3 */
            return ((seq_t)9 * (seq_t)k) + (seq_t)6;
    }
    /* unreachable but satisfies compiler */
    return 0;
}
