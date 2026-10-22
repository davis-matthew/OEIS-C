/*
 * A333382  Number of adjacent unequal parts in the n-th composition in
 *          standard-order.
 * https://oeis.org/A333382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of transitions between consecutive gaps of 1‑bits
 *            in the binary representation of n (equivalently, a(n) =
 *            A124767(n) – 1)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t count = 0;          /* number of adjacent unequal parts */
    idx_t last_one = 0;       /* position of previous 1‑bit */
    unsigned int last_gap = 0;/* size of previous gap */
    int first = 1;            /* flag for the first 1‑bit found */

    for (idx_t i = 0; i < 64; ++i) {
        if ((n >> i) & (idx_t)1) {
            if (first) {
                last_one = i;
                last_gap = (unsigned int)(i + 1);  /* distance from virtual -1 */
                first = 0;
            } else {
                unsigned int gap = (unsigned int)(i - last_one);
                if (gap != last_gap)
                    ++count;
                last_gap = gap;
                last_one = i;
            }
        }
    }
    return count;
}
