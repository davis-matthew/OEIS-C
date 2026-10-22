/*
 * A301710  Coordination sequence for node of type V2 in "krc" 2-D tiling (or
 *          net).
 * https://oeis.org/A301710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(2*t)=11*t, a(4*t+1)=22*t+5, a(4*t+3)=22*t+17 (for t≥0)
 * Range:     u8 n <= 46, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    if ((n & 1ULL) == 0) {                /* n = 2*t  */
        seq_t t = (seq_t)(n >> 1);
        return t * (seq_t)11;
    }

    /* n is odd */
    if ((n & 3ULL) == 1) {                /* n = 4*t+1 */
        seq_t t = (seq_t)((n - 1) / 4);
        return t * (seq_t)22 + (seq_t)5;
    } else {                              /* n = 4*t+3 */
        seq_t t = (seq_t)((n - 3) / 4);
        return t * (seq_t)22 + (seq_t)17;
    }
}
