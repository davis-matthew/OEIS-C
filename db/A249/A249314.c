/*
 * A249314  Number of length n+6 0..3 arrays with no seven consecutive terms
 *          having six times any element equal to the sum of the remaining
 *          six.
 * https://oeis.org/A249314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 2, u32 n <= 10, u64 n <= 28, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Counts length n+6 arrays over the alphabet {0,1,2,3} that avoid any
   seven‑consecutive window for which an element equals the sum of the
   remaining six elements (i.e. the window sum is 7 × that element).  The
   function implements a DP over the 6‑digit suffix of the sequence. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define STATE_COUNT 4096   /* 4^6 */

seq_t a(idx_t n)
{
    /* Lazy static transition table */
    static int initialized = 0;
    static int trans[STATE_COUNT][4];

    if (!initialized) {
        /* Pre‑compute transition table once */
        for (int s = 0; s < STATE_COUNT; ++s) {
            unsigned char sum = 0;
            unsigned char mask = 0;
            int t = s;
            for (int k = 0; k < 6; ++k) {
                unsigned char digit = (unsigned char)(t & 3);
                sum += digit;
                mask |= (1U << digit);
                t >>= 2;
            }
            for (int d = 0; d < 4; ++d) {
                unsigned char nsum = sum + (unsigned char)d;
                int ns = -1;
                if (nsum % 7 == 0) {
                    unsigned char target = nsum / 7;
                    /* Allowed only if target not present among the seven digits */
                    if (target < 4) {
                        unsigned char wmask = mask | (1U << d);
                        if (!(wmask & (1U << target))) {
                            ns = ((s >> 2) & 0x3FF) | (d << 10);
                        }
                    }
                } else {
                    ns = ((s >> 2) & 0x3FF) | (d << 10);
                }
                trans[s][d] = ns;
            }
        }
        initialized = 1;
    }

    /* DP arrays */
    static seq_t cur[STATE_COUNT];
    static seq_t next[STATE_COUNT];

    /* Initialise counts for length 6 (all 4^6 sequences are admissible) */
    for (int s = 0; s < STATE_COUNT; ++s)
        cur[s] = (seq_t)1;

    /* Iterate over the additional n digits to reach length n+6 */
    for (idx_t step = 0; step < n; ++step) {
        memset(next, 0, sizeof(next));

        for (int s = 0; s < STATE_COUNT; ++s) {
            seq_t val = cur[s];
            if (!val)
                continue;
            for (int d = 0; d < 4; ++d) {
                int ns = trans[s][d];
                if (ns >= 0)
                    next[ns] += val;
            }
        }

        memcpy(cur, next, sizeof(cur));
    }

    /* Sum over all admissible suffixes to obtain the total */
    seq_t total = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s)
        total += cur[s];

    return total;
}
