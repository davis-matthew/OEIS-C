/*
 * A375127  The anti-run-leader transformation for standard compositions.
 * https://oeis.org/A375127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index (in standard order) of the composition formed by
 *            the leaders of the anti‑runs of the n‑th composition in standard
 *            order
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n = 0 corresponds to the empty composition. */
    if (n == 0)
        return (seq_t)0;

    /* Find the positions of the 1‑bits in the reversed binary
       expansion of n (least significant bit has position 1). */
    int pos[65];
    int m = 0;                    /* number of 1-bits */
    idx_t tmp = n;
    int bit = 1;
    while (tmp) {
        if (tmp & 1ULL)
            pos[m++] = bit;
        tmp >>= 1;
        bit++;
    }

    /* Compute the differences of consecutive positions.
       The first difference is from position 0 to the first 1-bit. */
    int diff[65];
    for (int i = 0; i < m; ++i)
        diff[i] = (i == 0) ? pos[0] : pos[i] - pos[i - 1];

    /* The parts of the composition are the reverse of these differences. */
    int part[65];
    for (int i = 0; i < m; ++i)
        part[i] = diff[m - 1 - i];

    /* Extract leaders of anti‑runs: an element is a leader
       if it is the first element or if it equals the previous part. */
    int lead[65];
    int L = 0;
    for (int i = 0; i < m; ++i) {
        if (i == 0 || part[i] == part[i - 1])
            lead[L++] = part[i];
    }

    /* Compute the index of the leaders composition. */
    if (L == 0)
        return (seq_t)0;

    idx_t index = 0;
    idx_t cum = 0;
    for (int i = L - 1; i >= 0; --i) {
        cum += lead[i];
        index |= ((idx_t)1ULL << (unsigned)(cum - 1));
    }

    return (seq_t)index;
}
