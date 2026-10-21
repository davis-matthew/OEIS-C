/*
 * A249191  Number of length n+6 0..2 arrays with no seven consecutive terms
 *          having four times the sum of any three elements equal to three
 *          times the sum of the remaining four.
 * https://oeis.org/A249191
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 16, u64 n <= 39, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of length n+6 arrays over {0,1,2} in which no
 * seven consecutive terms have four times the sum of any three of
 * them equal to three times the sum of the remaining four.
 *
 * The computation is performed by a dynamic programming that
 * keeps the last six digits of the sequence as a state and
 * checks every new 7‑tuple against a precomputed table of valid
 * windows.  All arithmetic uses the seq_t type, which may be an
 * ordinary unsigned integer or an arbitrary‑precision type.
 */
#include <stdbool.h>

static bool valid_window[2187];   /* 3^7 */

static void init_valid_window(void)
{
    static bool initialized = false;
    if (initialized) return;
    initialized = true;

    for (int w = 0; w < 2187; ++w) {
        int digits[7];
        int tmp = w;
        /* extract 7 base‑3 digits */
        for (int i = 6; i >= 0; --i) {
            digits[i] = tmp % 3;
            tmp /= 3;
        }

        int sum = 0;
        for (int i = 0; i < 7; ++i) sum += digits[i];

        /* if total sum not divisible by 7, the window is automatically valid */
        if (sum % 7 != 0) {
            valid_window[w] = true;
            continue;
        }

        int target = 3 * sum / 7;
        bool bad = false;
        /* check all 7 choose 3 = 35 subsets */
        for (int i = 0; i < 7 && !bad; ++i)
            for (int j = i + 1; j < 7 && !bad; ++j)
                for (int k = j + 1; k < 7 && !bad; ++k) {
                    int s123 = digits[i] + digits[j] + digits[k];
                    if (s123 == target) bad = true;
                }

        valid_window[w] = !bad;
    }
}

seq_t a(idx_t n)
{
    init_valid_window();

    /* State size: 3^6 = 729 */
    const int STATES = 729;
    const int SHIFT_MASK = 243;   /* 3^5 */

    seq_t cur[STATES];
    seq_t nxt[STATES];

    /* Initial sequences of length 6: any 6‑tuple is allowed. */
    for (int s = 0; s < STATES; ++s) cur[s] = 1;

    idx_t L = n + 6;   /* total length of the array */

    for (idx_t pos = 6; pos < L; ++pos) {
        /* clear next array */
        for (int s = 0; s < STATES; ++s) nxt[s] = 0;

        for (int s = 0; s < STATES; ++s) {
            seq_t ways = cur[s];
            if (ways == 0) continue;

            /* try to append 0,1,2 */
            for (int v = 0; v < 3; ++v) {
                int widx = s * 3 + v;            /* 7‑tuple index */
                if (!valid_window[widx]) continue;

                int new_state = (s % SHIFT_MASK) * 3 + v;
                nxt[new_state] += ways;
            }
        }

        /* swap cur and nxt */
        for (int s = 0; s < STATES; ++s) cur[s] = nxt[s];
    }

    /* sum all final states */
    seq_t result = 0;
    for (int s = 0; s < STATES; ++s) result += cur[s];
    return result;
}
