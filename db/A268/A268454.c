/*
 * A268454  Number of length-n 0..5 arrays with no adjacent pair x,x+1
 *          followed at any distance by x+1,x.
 * https://oeis.org/A268454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A268454: Number of length-n arrays over {0,1,2,3,4,5}
   with no adjacent pair x,x+1 followed at any distance by x+1,x. */
#include <stdbool.h>

/* external types supplied by the build */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;

    const int PREV_STATES = 6;          /* symbols 0..5 */
    const int MASK_BITS   = 5;          /* rises for 0..4 */
    const int MASK_SIZE   = 1 << MASK_BITS; /* 32 */
    const int STATE_COUNT = PREV_STATES * MASK_SIZE; /* 192 */

    /* state index: prev * MASK_SIZE + mask */
    seq_t cur[STATE_COUNT];
    seq_t next[STATE_COUNT];

    /* initialize all to 0 */
    for (int i = 0; i < STATE_COUNT; ++i) cur[i] = 0;

    /* first position: any symbol with no rises seen */
    for (int symbol = 0; symbol < PREV_STATES; ++symbol) {
        cur[symbol * MASK_SIZE + 0] = 1;
    }

    /* iterate positions 2 .. n */
    for (idx_t pos = 2; pos <= n; ++pos) {
        /* zero next array */
        for (int i = 0; i < STATE_COUNT; ++i) next[i] = 0;

        /* transition */
        for (int prev = 0; prev < PREV_STATES; ++prev) {
            for (int mask = 0; mask < MASK_SIZE; ++mask) {
                seq_t val = cur[prev * MASK_SIZE + mask];
                if (!val) continue;                 /* no sequences reaching this state */

                for (int curr = 0; curr < PREV_STATES; ++curr) {
                    /* Check forbidden descent: (prev,curr) = (x+1,x) where rise x already seen */
                    if (curr + 1 == prev) { /* descent */
                        if (mask & (1 << curr)) continue; /* rise of same x seen earlier -> forbidden */
                    }

                    int newmask = mask;
                    /* If this pair is a rise (x,x+1) set corresponding bit */
                    if (prev + 1 == curr) {
                        newmask |= (1 << prev);
                    }

                    next[curr * MASK_SIZE + newmask] += val;
                }
            }
        }

        /* swap cur and next */
        for (int i = 0; i < STATE_COUNT; ++i) cur[i] = next[i];
    }

    /* sum all final states */
    seq_t total = 0;
    for (int i = 0; i < STATE_COUNT; ++i) total += cur[i];
    return total;
}
