/*
 * A375977  Number of subsets of {1,2,...,n} such that no two elements differ
 *          by 2 or 5.
 * https://oeis.org/A375977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of subsets of {1,…,n} such that no two elements differ by 2 or 5.
   The function uses a state‑machine DP: a 5‑bit mask encodes which
   of the last five positions (i‑5 … i‑1) were selected.  From each state
   we may skip the current position, or select it provided that i‑2
   (bit 1) and i‑5 (bit 4) were not chosen.  The algorithm runs in
   O(n) time with a fixed 32‑element table. */
seq_t a(idx_t n)
{
    /* 5‑bit sliding window (bits 0–4 correspond to i‑1 … i‑5) */
    const unsigned STATES = 32;
    seq_t cur[STATES], nxt[STATES];
    unsigned m;

    /* initialise: no positions selected yet */
    for (m = 0; m < STATES; ++m) cur[m] = 0;
    cur[0] = 1;

    for (idx_t i = 1; i <= n; ++i)
    {
        /* clear next state table */
        for (m = 0; m < STATES; ++m) nxt[m] = 0;

        for (m = 0; m < STATES; ++m)
        {
            seq_t cnt = cur[m];
            if (!cnt) continue;

            /* skip current element i */
            unsigned no_pick = (m << 1) & 0x1F;       /* shift window */
            nxt[no_pick] += cnt;

            /* pick i if positions i‑2 (bit 1) and i‑5 (bit 4) are free */
            if ((m & 0x02) == 0 && (m & 0x10) == 0)
            {
                unsigned pick = no_pick | 1;           /* set bit 0 */
                nxt[pick] += cnt;
            }
        }
        /* advance to next index */
        for (m = 0; m < STATES; ++m) cur[m] = nxt[m];
    }

    /* sum all end states */
    seq_t result = 0;
    for (m = 0; m < STATES; ++m) result += cur[m];
    return result;
}
