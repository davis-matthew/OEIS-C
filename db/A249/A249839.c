/*
 * A249839  Number of length n+4 0..3 arrays with no five consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A249839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 5, u32 n <= 16, u64 n <= 38, u128 n <= 83,
 *            big any n
 * Helpers:   forbidden5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A249839: number of length n+4 0..3 arrays with no five
 * consecutive terms having the maximum of any two terms equal to
 * the minimum of the remaining three terms.
 *
 * Computation: dynamic programming over the last four digits.
 * For each 4‑tuple (a,b,c,d) and next digit e the 5‑tuple
 * (a,b,c,d,e) is checked for the forbidden pattern.  The
 * transition (a,b,c,d) → (b,c,d,e) is allowed iff the pattern does
 * not occur.  The DP starts with all 4^4 possible prefixes of length
 * 4 (no constraints yet) and extends the array one element at a
 * time up to length n+4.  The answer is the sum over all final
 * states.
 *
 * Complexity: O((n+4)*256*4) time, O(256) memory.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/forbidden5.h"

seq_t a(idx_t n)
{
    /* precompute transition table once */
    static int16_t next_state[256][4];
    static bool init = false;
    if (!init) {
        init = true;
        unsigned char state_digits[4];
        for (int s = 0; s < 256; ++s) {
            int tmp = s;
            for (int i = 3; i >= 0; --i) {
                state_digits[i] = tmp & 3;
                tmp >>= 2;
            }
            for (int e = 0; e < 4; ++e) {
                if (forbidden5(state_digits, (unsigned char)e)) {
                    next_state[s][e] = -1;
                } else {
                    /* next state: shift left 2 bits, insert e */
                    int ns = ((s & 0x3F) << 2) | e;
                    next_state[s][e] = (int16_t)ns;
                }
            }
        }
    }

    /* DP arrays */
    static seq_t dp[2][256];
    int cur = 0, nxt = 1;

    /* initialize length 4 prefix: all 4^4 = 256 possibilities */
    for (int i = 0; i < 256; ++i) dp[cur][i] = 1;

    /* target length */
    idx_t L = n + 4;   /* n >= 1, so L >= 5 */

    for (idx_t len = 5; len <= L; ++len) {
        for (int i = 0; i < 256; ++i) dp[nxt][i] = 0;
        for (int s = 0; s < 256; ++s) {
            seq_t val = dp[cur][s];
            if (!val) continue;
            for (int e = 0; e < 4; ++e) {
                int16_t ns = next_state[s][e];
                if (ns >= 0) dp[nxt][ns] += val;
            }
        }
        int tmp = cur; cur = nxt; nxt = tmp;
    }

    /* sum all final states */
    seq_t ans = 0;
    for (int i = 0; i < 256; ++i) ans += dp[cur][i];
    return ans;
}
