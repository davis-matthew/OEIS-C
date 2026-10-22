/*
 * A347144  a(n) is the smallest number whose square uses all the digits but
 *          n.
 * https://oeis.org/A347144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search for each excluded digit (index % 10). */
/* For each d in 0..9 find the smallest x such that x² contains every digit
   except d at least once.  The results are cached and returned for any n. */

#include <stdbool.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* small cache for the ten possible excluded digits */
    static bool ready[10] = { false };
    static seq_t ans[10];
    static bool init = false;

    if (!init) {
        init = true;
        seq_t cand = 1;
        /* keep searching until all ten excluded‑digit cases are found */
        while (true) {
            seq_t sq = cand * cand;
            unsigned int freq[10] = {0};
            digit_frequencies(sq, freq);

            bool all_done = true;
            for (int d = 0; d < 10; ++d) {
                if (!ready[d]) {
                    all_done = false;
                    bool ok = true;
                    for (int i = 0; i < 10; ++i) {
                        if (i == d) {
                            if (freq[i] != 0) { ok = false; break; }
                        } else {
                            if (freq[i] == 0) { ok = false; break; }
                        }
                    }
                    if (ok) {
                        ans[d]      = cand;
                        ready[d]    = true;
                    }
                }
            }

            if (all_done)
                break;                   /* all 10 numbers found */
            ++cand;
        }
    }

    /* index beyond 9 cycles the excluded digit: use n % 10 */
    int d = (int)(n % 10);
    return ans[d];
}
