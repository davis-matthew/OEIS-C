/*
 * A261441  Number of binary strings of length n+3 such that the smallest
 *          number whose binary representation is not visible in the string is
 *          5.
 * https://oeis.org/A261441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of binary strings of length n+3 that contain * the
 *            binary strings of 0,1,2,3,4 (i.e. "0","1","10","11","100") as *
 *            substrings but do NOT contain "101" (the binary representation
 *            of 5). * Computation is performed by a dynamic‑programming
 *            automaton that * tracks the last two bits (to...
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 36, u64 n <= 75, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* automaton states: last two bits of the string, encoded as
     * 0 -> 00, 1 -> 01, 2 -> 10, 3 -> 11.  Start from state 0
     * (assumes missing leading bits are treated as 0). */
    const int STATES = 4;
    /* 5 required patterns → 5‑bit mask: bit0=0, bit1=1, bit2=10,
     * bit3=11, bit4=100.  All 5 set is value 31. */
    const int MASKS  = 32;
    seq_t cur[STATES][MASKS];
    seq_t nxt[STATES][MASKS];

    /* initialise all counts to zero */
    for (int s = 0; s < STATES; ++s)
        for (int m = 0; m < MASKS; ++m)
            cur[s][m] = 0;
    cur[0][0] = 1;                 /* empty string */

    idx_t L = n + 3;                /* total length of the string */

    for (idx_t pos = 0; pos < L; ++pos) {
        /* clear nxt table */
        for (int s = 0; s < STATES; ++s)
            for (int m = 0; m < MASKS; ++m)
                nxt[s][m] = 0;

        for (int s = 0; s < STATES; ++s) {
            for (int m = 0; m < MASKS; ++m) {
                seq_t cnt = cur[s][m];
                if (!cnt) continue;

                for (int b = 0; b < 2; ++b) {
                    /* forbidden: appending '1' after suffix '10' would create "101" */
                    if (s == 2 && b == 1) continue;

                    int nm = m;        /* new mask of seen patterns */

                    /* single‑bit patterns */
                    if (b == 0) nm |= 1; /* "0" seen */
                    else        nm |= 2; /* "1" seen */

                    /* two‑bit patterns depending on last bit of current suffix */
                    if ((s & 1) == 1 && b == 0) nm |= 4; /* "10" */
                    if ((s & 1) == 1 && b == 1) nm |= 8; /* "11" */

                    /* three‑bit pattern "100" */
                    if (s == 2 && b == 0) nm |= 16;      /* "100" */

                    /* new suffix after adding b */
                    int ns = ((s & 1) << 1) | b;

                    nxt[ns][nm] += cnt;
                }
            }
        }

        /* copy nxt back to cur for next iteration */
        for (int s = 0; s < STATES; ++s)
            for (int m = 0; m < MASKS; ++m)
                cur[s][m] = nxt[s][m];
    }

    seq_t ans = 0;
    const int FULL = 31;            /* all five patterns present */
    for (int s = 0; s < STATES; ++s)
        ans += cur[s][FULL];

    return ans;
}
