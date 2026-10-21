/*
 * A272604  Maximum subrange sum over n written out in binary with -1 for each
 *          zero (cf. A276691).
 * https://oeis.org/A272604
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum subrange sum of +1 for every 1 bit and -1 for
 *            every 0 bit * in the binary representation of n
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Find index of most significant 1 bit (0‑based from LSB). */
    idx_t msb = 0;
    idx_t tmp  = n;
    while (tmp > 0) {
        ++msb;
        tmp >>= 1;
    }
    --msb;                     /* msb is now the highest set bit index */

    long long current = 0;
    long long best    = 0;

    /* Kadane over the bits from MSB down to LSB. */
    for (idx_t i = msb; ; --i) {
        long long val = ((n >> i) & (idx_t)1) ? 1 : -1;
        current = (current > 0) ? current + val : val;
        if (current > best) best = current;
        if (i == 0) break;     /* LSB processed */
    }

    return (seq_t)best;
}
