/*
 * A344193  Numbers that are the sum of four fourth powers in exactly two
 *          ways.
 * https://oeis.org/A344193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 9300, u64 n <= 9300, u128 n <= 9300,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A344193  Numbers that are the sum of four fourth powers in exactly two ways
   Computation: enumerate all unordered quadruples a≤b≤c≤d≥1, count the number of
   representations for each sum, and collect those sums that occur exactly twice. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Compute a^4 using seq_t arithmetic */
static inline seq_t fourth_power(idx_t a) {
    seq_t t = (seq_t)a;
    t *= t;            /* a² */
    t *= t;            /* a⁴ */
    return t;
}

/* Global storage for generated terms.  Lazily built on first call. */
static seq_t *global_seq   = NULL;   /* array of terms */
static size_t global_len   = 0;      /* number of terms */
static bool built = false;           /* flag telling whether the sequence has been built */

/* Build the sequence until at least TARGET_TERMS terms are available. */
static void build_sequence(void)
{
    const size_t TARGET_TERMS = 10000;          /* expected number of terms */
    idx_t limit = 30000000;                     /* start with 30 million */

    while (true) {
        /* Find maximal i with i^4 <= limit. */
        idx_t max_i = 0;
        while (fourth_power(max_i) <= limit) max_i++;
        max_i--;                                /* last valid i */

        /* Pre‑compute fourth powers up to max_i. */
        seq_t *pow4 = (seq_t *)malloc((size_t)(max_i + 1) * sizeof(seq_t));
        if (!pow4) return;                     /* allocation failure – give up */
        pow4[0] = 0;
        for (idx_t i = 1; i <= max_i; ++i) pow4[i] = fourth_power(i);

        /* Count the number of unordered quadruples for each sum. */
        unsigned char *cnt = (unsigned char *)
            malloc((size_t)(limit + 1) * sizeof(unsigned char));
        if (!cnt) { free(pow4); return; }
        memset(cnt, 0, (size_t)(limit + 1));

        for (idx_t a = 1; a <= max_i; ++a) {
            for (idx_t b = a; b <= max_i; ++b) {
                seq_t ab = pow4[a] + pow4[b];
                if (ab > limit) break;
                for (idx_t c = b; c <= max_i; ++c) {
                    seq_t abc = ab + pow4[c];
                    if (abc > limit) break;
                    for (idx_t d = c; d <= max_i; ++d) {
                        seq_t s = abc + pow4[d];
                        if (s > limit) break;
                        if (cnt[(idx_t)s] < 3) cnt[(idx_t)s]++;   /* cap at 3 */
                    }
                }
            }
        }

        /* Collect sums that appear exactly twice. */
        size_t cap = 0, len = 0;
        seq_t *tmp = NULL;
        for (idx_t s = 0; s <= limit; ++s) {
            if (cnt[s] == 2) {
                if (len >= cap) {
                    cap = cap ? cap * 2 : 1024;
                    seq_t *new_tmp = (seq_t *)realloc(tmp, cap * sizeof(seq_t));
                    if (!new_tmp) { free(tmp); free(cnt); free(pow4); return; }
                    tmp = new_tmp;
                }
                tmp[len++] = (seq_t)s;
            }
        }

        free(cnt);
        free(pow4);

        if (len >= TARGET_TERMS) {
            global_seq = tmp;
            global_len = len;
            built = true;
            break;
        } else {
            free(tmp);                       /* not enough terms; enlarge limit */
            limit <<= 1;                     /* double the limit */
        }
    }
}

/* Required function: retrieve the nth term (1‑based). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!built) build_sequence();
    if (!built || (size_t)n > global_len) return 0;
    return global_seq[n - 1];
}
