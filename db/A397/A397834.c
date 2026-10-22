/*
 * A397834  Number of binary words of length n containing the consecutive word
 *          ababab.
 * https://oeis.org/A397834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n – number of binary words of length n that avoid the
 *            substring "ababab". * The avoidance count is computed by a
 *            finite‑state automaton derived from the pattern
 * Range:     u8 n <= 11, u16 n <= 18, u32 n <= 33, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const unsigned char trans[6][2] = {
    /* state 0 */ {1, 0},
    /* state 1 */ {1, 2},
    /* state 2 */ {3, 0},
    /* state 3 */ {1, 4},
    /* state 4 */ {5, 0},
    /* state 5 */ {1, 6} /* 6 denotes the forbidden full match */
};

seq_t a(idx_t n)
{
    /* Avoiders of length n (states 0..5), start with empty word in state 0 */
    seq_t cur[6] = {0};
    cur[0] = 1;
    for (idx_t i = 0; i < n; ++i) {
        seq_t nxt[6] = {0};
        for (int s = 0; s < 6; ++s) {
            seq_t val = cur[s];
            if (!val) continue;
            unsigned ns = trans[s][0];      /* add 'a' */
            if (ns < 6) nxt[ns] += val;
            ns = trans[s][1];               /* add 'b' */
            if (ns < 6) nxt[ns] += val;
        }
        for (int s = 0; s < 6; ++s) cur[s] = nxt[s];
    }

    /* Total avoiders */
    seq_t avoid = 0;
    for (int s = 0; s < 6; ++s) avoid += cur[s];

    /* Total words of length n: 2^n */
    seq_t power = 1;
    for (idx_t i = 0; i < n; ++i) power <<= 1;   /* multiply by 2 */

    /* Result: words that contain the pattern at least once */
    return power - avoid;
}
