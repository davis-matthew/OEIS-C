/*
 * A250074  Number of length n+5 0..1 arrays with every six consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining four terms.
 * https://oeis.org/A250074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 33, u64 n <= 72, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A250074: Number of length n+5 binary arrays with every 6 consecutive terms not having exactly four 1's. */
seq_t a(idx_t n)
{
    idx_t L = n + 5;   /* actual array length */

    /* For lengths shorter than 6 no window exists, all binary arrays are valid. */
    if (L < 6) {
        seq_t total = 1;
        for (idx_t i = 0; i < L; ++i)
            total = total * 2;
        return total;
    }

    /* popcount for all 5‑bit states */
    static const unsigned popcnt[32] = {
        0,1,1,2,1,2,2,3,1,2,2,3,2,3,3,4,
        1,2,2,3,2,3,3,4,2,3,3,4,3,4,4,5
    };

    seq_t cur[32];
    seq_t nxt[32];

    /* initialise for length 5 (any 5‑bit prefix is possible) */
    for (int i = 0; i < 32; ++i) cur[i] = 1;

    /* iterate over each additional bit up to length L */
    for (idx_t len = 6; len <= L; ++len) {
        for (int i = 0; i < 32; ++i) nxt[i] = 0;

        for (int s = 0; s < 32; ++s) {
            seq_t val = cur[s];
            if (val == 0) continue;

            unsigned p = popcnt[s];

            /* append bit 0 */
            if (p != 4) {
                int ns = (s << 1) & 0x1F;          /* new 5‑bit state */
                nxt[ns] += val;
            }
            /* append bit 1 */
            if (p + 1 != 4) {
                int ns = ((s << 1) | 1) & 0x1F;    /* new 5‑bit state */
                nxt[ns] += val;
            }
        }
        /* move next to current */
        for (int i = 0; i < 32; ++i) cur[i] = nxt[i];
    }

    /* sum all valid states */
    seq_t total = 0;
    for (int i = 0; i < 32; ++i)
        total += cur[i];

    return total;
}
