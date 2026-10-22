/*
 * A394533  Binary expansion of a(n) obtained from binary expansion of n with
 *          increased number of zeros separated by every next 1 by total
 *          number of zeros lying from the left side of selected 1.
 * https://oeis.org/A394533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): Transform the binary expansion of n.
   For each 1-bit, output that 1 followed by the zeros that
   originally followed this 1 in n, plus an additional number of
   zeros equal to the total number of zeros to the left of that
   1 in n.  The function works for any idx_t (unsigned long long)
   and produces a seq_t that may be an arbitrary‑precision integer. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            // offset 1; treat n==0 harmlessly

    /* collect indices of 1‑bits in descending order (MSB → LSB) */
    idx_t pos[64];
    int m = 0;
    for (int bit = 63; bit >= 0; --bit)
        if ((n >> bit) & 1)
            pos[m++] = bit;

    seq_t res = 0;
    seq_t zeros_prefix = 0;          // total zeros to the left of current 1

    for (int j = 0; j < m; ++j) {
        idx_t cur = pos[j];
        seq_t zeros_original;
        if (j + 1 < m)
            zeros_original = (seq_t)(cur - pos[j + 1] - 1);   // original zeros between this 1 and the next
        else
            zeros_original = (seq_t)cur;                     // zeros after the last 1

        seq_t zeros_total = zeros_original + zeros_prefix;   // total zeros to append here

        res = (res << 1) | (seq_t)1;                        // append the 1
        if (zeros_total)
            res <<= (unsigned long long)zeros_total;           // append the zeros

        if (j + 1 < m) {                                    // update zeros_prefix for next 1
            seq_t between = (seq_t)(cur - pos[j + 1] - 1);
            zeros_prefix += between;
        }
    }

    return res;
}
