/*
 * A269917  Number of ternary strings of length n with maximal run length five
 *          containing 111112.
 * https://oeis.org/A269917
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) counts ternary strings of length n with maximal run length
 *            five containing 111112. The computation is performed by a simple
 *            DP over the last symbol, the current run length (≤5), and a flag
 *            indicating whether the pattern has already appeared
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 6) return 0;   /* terms below offset are undefined */

    const int STATES = 30; /* 3 symbols * 5 run lengths * 2 flags */
    seq_t cur[STATES];
    seq_t next[STATES];

    /* initialise all states to zero */
    for (int i = 0; i < STATES; ++i) cur[i] = 0;

    /* length 1 base cases: one string for each symbol with run length 1, flag 0 */
    for (int s = 0; s < 3; ++s) cur[s * 10] = 1;

    /* iterate lengths 2 .. n */
    for (idx_t len = 2; len <= n; ++len) {
        /* zero next array */
        for (int i = 0; i < STATES; ++i) next[i] = 0;

        /* transition from every existing state */
        for (int idx = 0; idx < STATES; ++idx) {
            seq_t val = cur[idx];
            if (val == 0) continue;                 /* skip empty state */

            int s  = idx / 10;                       /* last symbol 0..2 */
            int rem = idx - s * 10;                  /* remainder (l-1)*2 + f */
            int l  = rem / 2 + 1;                    /* run length 1..5 */
            int f  = rem % 2;                        /* flag 0/1 */

            /* try to append each possible next symbol c */
            for (int c = 0; c < 3; ++c) {
                int lnew = (c == s) ? l + 1 : 1;     /* new run length */
                if (lnew > 5) continue;             /* violate maximal run length */

                int fnew = f;
                if (s == 1 && l == 5 && c == 2)      /* pattern 111112 occurs */
                    fnew = 1;

                int idxnew = c * 10 + (lnew - 1) * 2 + fnew;
                next[idxnew] += val;
            }
        }

        /* move next to cur for next iteration */
        for (int i = 0; i < STATES; ++i) cur[i] = next[i];
    }

    /* sum all states with flag = 1 (pattern seen) */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i)
        if (i & 1) /* last bit of idx encodes flag */
            result += cur[i];

    return result;
}
