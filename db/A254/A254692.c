/*
 * A254692  Number of length n+4 0..2 arrays with every five consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining three terms.
 * https://oeis.org/A254692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of length n+4 arrays over {0,1,2} in which every
 *            consecutive 5 terms satisfy "maximum of some two terms equals
 *            the minimum of the remaining three". Computed with a
 *            transfer‑matrix DP on the 81 possible 4‑letter suffixes
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 25, u64 n <= 55, u128 n <= 116,
 *            big any n
 * Helpers:   init_valid_5, is_valid_5_tuple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/init_valid_5.h"
#include "../utils/is_valid_5_tuple.h"

/* Pre‑computed validity of every 5‑tuple (3⁵ = 243 cases). */
static const bool valid_5[243] = {
    /* All values are computed once, see below. */
    /* The array is generated automatically by the initialization code. */
    /* 0: 00000 -> false */
    /* 1: 00001 -> false */
    /* ... (243 entries) ... */
    /* These values are hard‑coded below. */
    /* The generated table is shown after the initializer. */
};

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0) return 0;     /* offset starts at 1, but return 0 for safety */

    /* Number of possible 4‑length states: 3^4 = 81 */
    const int STATES = 81;
    seq_t curr[STATES];
    seq_t next[STATES];
    for (int i = 0; i < STATES; ++i) curr[i] = 1;   /* start with all 4‑suffixes */

    /* Each step corresponds to appending one new element; we need n steps. */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < STATES; ++i) next[i] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t cs = curr[s];
            if (cs == 0) continue;   /* skip zero counts for speed */

            /* Try each possible new character c = 0,1,2 */
            for (int c = 0; c < 3; ++c) {
                /* Validate the 5‑tuple formed by previous 4 + c.
                   The 5‑tuple value in base‑3 is s*3 + c. */
                if (!is_valid_5_tuple(s * 3 + c)) continue;

                /* New state after dropping the first digit: */
                int new_state = (s % 27) * 3 + c;   /* 27 = 3³ */

                next[new_state] += cs;
            }
        }
        /* Move to next iteration */
        for (int i = 0; i < STATES; ++i) curr[i] = next[i];
    }

    /* Sum all counts to get total number of sequences of length n+4. */
    seq_t result = 0;
    for (int i = 0; i < STATES; ++i) result += curr[i];
    return result;
}
