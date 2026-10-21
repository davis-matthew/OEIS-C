/*
 * A284964  Numbers with digits 3 and 9 only.
 * https://oeis.org/A284964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal integer whose digits are only 3 and 9. The digits
 *            are obtained by writing (n+1) in binary, discarding the leading
 *            1, then replacing each remaining bit: 0 → 3, 1 → 9
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t m = n + 1;          /* m has a leading 1 that we will skip   */

    /* Find position of most significant 1-bit in m (0‑based). */
    idx_t msb = 0;
    while ((m >> (msb + 1)) != 0) {
        ++msb;
    }
    /* msb is at least 1 because m ≥ 2.  The highest bit itself is
       the discarded leading 1; we process the remaining bits from
       msb‑1 down to 0. */

    seq_t result = 0;
    for (idx_t i = msb; i-- > 0; ) {   /* i goes msb-1 … 0 */
        if (m & ((idx_t)1 << i)) {
            result = result * 10 + 9;   /* bit 1 → digit 9 */
        } else {
            result = result * 10 + 3;   /* bit 0 → digit 3 */
        }
    }
    return result;
}
