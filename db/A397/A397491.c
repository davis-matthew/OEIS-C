/*
 * A397491  a(n) = (n^13 - n)/2730.
 * https://oeis.org/A397491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^13 - n) / 2730
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If seq_t is a small integer type (≤64 bits) use a 128‑bit
     * intermediate to avoid overflow of the intermediate power.
     * For arbitrary‑precision seq_t types we rely on its native
     * arithmetic. */
    if (sizeof(seq_t) <= sizeof(unsigned long long)) {
        unsigned __int128 t = 1;
        for (uint64_t i = 0; i < 13; ++i)
            t *= (unsigned __int128)n;
        t -= n;          /* n^13 - n */
        t /= 2730;       /* divide by denominator */
        return (seq_t)t; /* truncate to seq_t; OK when result fits */
    } else {
        seq_t sn = (seq_t)n;
        seq_t res = sn;          /* start with n^1 */
        for (int i = 1; i < 13; ++i)
            res = res * sn;      /* compute n^13 */
        res = res - sn;          /* n^13 - n */
        seq_t denom = (seq_t)2730;
        return res / denom;      /* exact division, result fits in seq_t */
    }
}
