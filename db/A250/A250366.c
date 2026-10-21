/*
 * A250366  Number of length n+6 0..1 arrays with every seven consecutive
 *          terms having the maximum of some three terms equal to the minimum
 *          of the remaining four terms.
 * https://oeis.org/A250366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 34, u64 n <= 76, u128 n <= 159,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A250366
 * Number of length n+6 0..1 arrays with every seven consecutive terms having the
 * maximum of some three terms equal to the minimum of the remaining four terms.
 *
 * This is equivalent to counting binary strings of length L = n+6 that contain
 * no run of 7 consecutive bits with exactly four 1‑bits.
 *
 * The DP below keeps, for each 6-bit suffix, the number of partial sequences
 * that end with that suffix.  When extending by one bit we form a new window
 * of 7 bits and reject it if it contains exactly 4 ones.  Complexity is
 * O(n·64) and works for any allowed seq_t and idx_t.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* popcount of all 6-bit numbers, computed once */
    static unsigned char pop[64];
    static int pop_ready = 0;
    if (!pop_ready) {
        for (int i = 0; i < 64; ++i) {
            unsigned char c = 0;
            unsigned int v = i;
            while (v) { c += v & 1; v >>= 1; }
            pop[i] = c;
        }
        pop_ready = 1;
    }

    /* DP arrays for 6‑bit suffixes */
    seq_t dp[64];
    seq_t dp_next[64];

    /* initial suffixes after 6 bits: all possibilities are allowed */
    for (int s = 0; s < 64; ++s) dp[s] = (seq_t)1;

    /* iterate over the positions 7 … n+6 */
    idx_t limit = n + 6;           /* length of the final sequence */
    for (idx_t pos = 7; pos <= limit; ++pos) {
        /* reset next array */
        for (int s = 0; s < 64; ++s) dp_next[s] = (seq_t)0;

        /* transition */
        for (int s = 0; s < 64; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;
            unsigned char ones_in_prefix = pop[s];

            /* try appending bit 0 or 1 */
            for (int bit = 0; bit <= 1; ++bit) {
                unsigned char ones = ones_in_prefix + (unsigned char)bit;
                if (ones == 4) continue;        /* forbidden window */
                int ns = ((s << 1) | bit) & 0x3F;  /* new 6‑bit suffix */
                dp_next[ns] = dp_next[ns] + val;
            }
        }

        /* copy back for next step */
        for (int s = 0; s < 64; ++s) dp[s] = dp_next[s];
    }

    /* sum over all suffixes gives the answer */
    seq_t ans = (seq_t)0;
    for (int s = 0; s < 64; ++s) ans = ans + dp[s];
    return ans;
}
