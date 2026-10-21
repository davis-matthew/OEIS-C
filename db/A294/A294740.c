/*
 * A294740  Numbers that are the sum of 5 nonzero squares in exactly 6 ways.
 * https://oeis.org/A294740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A294740.
   A294740: Numbers that are the sum of 5 non‑zero squares in exactly 6 ways.
   The sequence is finite; all terms lie ≤ 5408.
   The implementation enumerates all unordered 5‑tuples of non‑zero integers,
   counts the number of representations for each sum, collects the sums
   that occur exactly six times, and returns the n‑th such sum.
   For indices beyond the list length the function returns 0. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"  /* for seq_alloc_new_array() */

static bool initialized = false;
static seq_t *terms = NULL;
static idx_t term_count = 0;

seq_t a(idx_t n)
{
    if (!initialized) {
        /* Pre‑compute all terms up to the proven limit 5408. */
        const idx_t LIMIT = 5408U;
        const idx_t MAX_ROOT = 73U;   /* since 73² = 5329 ≤ 5408 < 74² */

        /* Array for counting representations of each sum. */
        unsigned int *cnt = (unsigned int*)calloc((size_t)(LIMIT + 1), sizeof(unsigned int));
        if (!cnt) return 0;          /* allocation failure */

        /* Pre‑compute squares 1² … 73². */
        int sq[74];
        for (idx_t i = 0; i <= MAX_ROOT; ++i)
            sq[i] = (int)(i * i);

        /* Enumerate unordered 5‑tuples (a ≥ b ≥ c ≥ d ≥ e ≥ 1). */
        for (idx_t a = 1; a <= MAX_ROOT; ++a) {
            int sa = sq[a];
            for (idx_t b = 1; b <= a; ++b) {
                int sab = sa + sq[b];
                if ((unsigned)sab > LIMIT) continue;
                for (idx_t c = 1; c <= b; ++c) {
                    int sabc = sab + sq[c];
                    if ((unsigned)sabc > LIMIT) continue;
                    for (idx_t d = 1; d <= c; ++d) {
                        int sabcd = sabc + sq[d];
                        if ((unsigned)sabcd > LIMIT) continue;
                        for (idx_t e = 1; e <= d; ++e) {
                            int sum = sabcd + sq[e];
                            if (sum > LIMIT) continue;
                            ++cnt[sum];
                        }
                    }
                }
            }
        }

        /* Count how many sums have exactly 6 representations. */
        idx_t len = 0;
        for (idx_t i = 1; i <= LIMIT; ++i)
            if (cnt[i] == 6) ++len;

        /* Allocate the list of terms. */
        if (len > 0) {
            terms = seq_alloc_new_array((size_t)len);
            if (terms) {
                idx_t pos = 0;
                for (idx_t i = 1; i <= LIMIT; ++i)
                    if (cnt[i] == 6) {
                        terms[pos] = (seq_t)i;
                        ++pos;
                    }
                term_count = len;
            }
        }

        free(cnt);
        initialized = true;
    }

    if (n >= 1 && n <= term_count)
        return terms[n - 1];
    else
        return 0;   /* no such term beyond the list's length */
}
