/*
 * A242206  Number of length n binary words which contain 00 and 01 and 10 and
 *          11 as (possibly overlapping) contiguous subsequences.
 * https://oeis.org/A242206
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = # of binary words of length n containing all four 2‑bit
 *            patterns 00,01,10,11. Computed by DP over states (mask,last)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define STATES 32          /* 2*16 possible combinations of (mask,last) */
#define FULL_MASK 15       /* 1111b – all four patterns seen */

seq_t a(idx_t n)
{
    if (n < 5) return 0;          /* below offset – undefined, return 0 */

    /* DP arrays for current and next length. Size is a compile‑time constant. */
    seq_t dp_cur[STATES];
    seq_t dp_next[STATES];

    /* initialise for length 1 */
    for (int i = 0; i < STATES; ++i) {
        dp_cur[i] = 0;
    }
    dp_cur[0] = 1;  /* mask 0, last 0 */
    dp_cur[1] = 1;  /* mask 0, last 1 */

    /* iterate lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        for (int i = 0; i < STATES; ++i) {
            dp_next[i] = 0;
        }
        for (int s = 0; s < STATES; ++s) {
            seq_t val = dp_cur[s];
            if (val == 0) continue;

            int mask = s >> 1;       /* current mask of pairs seen   */
            int last = s & 1;        /* last bit of the current word */

            /* append a 0 */
            {
                int new_last = 0;
                int pair = (last << 1) | new_last;          /* 0..3 */
                int new_mask = mask | (1 << pair);
                int new_state = (new_mask << 1) | new_last;
                dp_next[new_state] += val;
            }

            /* append a 1 */
            {
                int new_last = 1;
                int pair = (last << 1) | new_last;
                int new_mask = mask | (1 << pair);
                int new_state = (new_mask << 1) | new_last;
                dp_next[new_state] += val;
            }
        }
        /* move to next length */
        for (int i = 0; i < STATES; ++i) {
            dp_cur[i] = dp_next[i];
        }
    }

    /* sum over states that have seen all four patterns */
    seq_t result = 0;
    for (int last = 0; last < 2; ++last) {
        int state = (FULL_MASK << 1) | last;
        result += dp_cur[state];
    }
    return result;
}
