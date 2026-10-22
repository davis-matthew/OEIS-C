/*
 * A390991  Sum of first sums of the n-th composition in standard order.
 * https://oeis.org/A390991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2*A029837(n) - A001511(n) - A065120(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }

    idx_t first_pos = 0;   /* position of the first 1-bit (from LSB) */
    idx_t prev_pos  = 0;   /* position of the second‑to‑last 1-bit */
    idx_t last_pos  = 0;   /* position of the last 1-bit */
    idx_t pos       = 1;   /* LSB position index (1‑based) */

    idx_t x = n;
    while (x != 0) {
        if (x & 1) {
            if (first_pos == 0) {
                first_pos = pos;          /* first 1‑bit encountered */
            } else {
                prev_pos = last_pos;      /* update second‑to‑last */
            }
            last_pos = pos;               /* current 1‑bit becomes last */
        }
        x >>= 1;
        ++pos;
    }

    /* a(n) = last_pos - first_pos + prev_pos */
    seq_t result = (seq_t)last_pos - (seq_t)first_pos + (seq_t)prev_pos;
    return result;
}
