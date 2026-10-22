/*
 * A322149  In the binary representation of n, replace each run of k 0's (or
 *          1's) with k^2 0's (or 1's).
 * https://oeis.org/A322149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = replace each run of k identical bits in binary(n) by k^2
 *            identical bits (run‑length square transformation)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 62, u64 n <= 255,
 *            u128 n <= 4094, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find position of the most significant set bit. */
    idx_t msb = 63;
    while (msb > 0 && ((n >> msb) & 1ULL) == 0)
        msb--;

    seq_t  res     = 0;
    unsigned cur_bit = (n >> msb) & 1ULL;
    idx_t  runlen   = 1;

    /* Scan bits from the one below the MSB down to 0. */
    for (idx_t i = msb; i > 0; --i) {
        unsigned bit = (n >> (i - 1)) & 1ULL;
        if (bit == cur_bit) {
            ++runlen;
        } else {
            idx_t len2 = runlen * runlen;
            for (idx_t j = 0; j < len2; ++j)
                res = (res << 1) | cur_bit;
            cur_bit = bit;
            runlen  = 1;
        }
    }

    /* Flush the last run. */
    idx_t len2 = runlen * runlen;
    for (idx_t j = 0; j < len2; ++j)
        res = (res << 1) | cur_bit;

    return res;
}
