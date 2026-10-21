/*
 * A242629  Number of n-length words w over a 6-ary alphabet {a_1,...,a_6}
 *          such that w contains never more than j consecutive letters a_j
 *          (for 1<=j<=6).
 * https://oeis.org/A242629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n≥1, a(n) is the number of length‑n words over a
 *            6‑ary alphabet {a₁,…,a₆} in which the letter aⱼ never appears in
 *            a run longer than j. This is computed by a 21‑state DP: each
 *            state (j,r) means the word ends with letter aⱼ in a run of
 *            length r (1 ≤ r ≤ j). The DP transitions extend the...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */

static const int LIMITS[6] = {1, 2, 3, 4, 5, 6};
static const int OFFSETS[6] = {0, 1, 3, 6,10,15};

static const int STATE_J[21] = {
    0, 1, 1, 2, 2, 2, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5
};

static const int STATE_R[21] = {
     1, 1, 2, 1, 2, 3, 1, 2, 3, 4,
     1, 2, 3, 4, 5, 1, 2, 3, 4, 5, 6
};

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* State arrays for the current and next step */
    seq_t cur[21] = {0};
    seq_t nxt[21] = {0};

    /* Initialise length‑1 words: one state per letter */
    for (int j = 0; j < 6; ++j)
        cur[OFFSETS[j]] = (seq_t)1;

    /* DP for lengths 2 … n */
    for (idx_t step = 2; step <= n; ++step) {
        for (int i = 0; i < 21; ++i)
            nxt[i] = (seq_t)0;

        for (int i = 0; i < 21; ++i) {
            seq_t val = cur[i];
            if (!val)
                continue;

            int j = STATE_J[i];
            int r = STATE_R[i];

            /* Same letter, extend run if allowed */
            if (r < LIMITS[j]) {
                int idx = OFFSETS[j] + r;   /* r+1 */
                nxt[idx] += val;
            }

            /* Start a run of a different letter */
            for (int m = 0; m < 6; ++m) {
                if (m == j)
                    continue;
                int idx = OFFSETS[m];
                nxt[idx] += val;
            }
        }

        /* Prepare for next iteration */
        for (int i = 0; i < 21; ++i)
            cur[i] = nxt[i];
    }

    /* Sum all state counts to obtain a(n) */
    seq_t total = (seq_t)0;
    for (int i = 0; i < 21; ++i)
        total += cur[i];

    return total;
}
