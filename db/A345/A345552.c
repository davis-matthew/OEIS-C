/*
 * A345552  Numbers that are the sum of ten cubes in four or more ways.
 * https://oeis.org/A345552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A345552: numbers that are the sum of ten cubes in four or more distinct ways.
   The function enumerates all sums of exactly ten non‑negative cubes up to an
   adaptively chosen limit and returns the n‑th such number (1‑based index). */
#include <stdint.h>
#include <stdlib.h>

/* ---------- static state ---------- */
static seq_t *seq_buf = NULL;   /* array of values that satisfy the property   */
static size_t seq_len = 0;      /* number of values stored in seq_buf         */

#define SEARCH_LIMIT 200000          /* upper bound needed for the first ~10 000 terms */

static void compute_terms(void)
{
    const int max_used = 10;
    const size_t max_sum = SEARCH_LIMIT;

    /* flattened 2‑D array: [used][sum] -> number of ways (capped at 4)     */
    unsigned char *dp = (unsigned char *)calloc((max_used + 1) * (max_sum + 1), sizeof(unsigned char));
    if (!dp) return; /* allocation failure – abort silently */

    #define DP(u,s) (dp[(u) * (max_sum + 1) + (s)])

    DP(0,0) = 1; /* base case: zero cubes sum to zero */

    /* iterate over all possible cube values d³ ≤ SEARCH_LIMIT */
    for (idx_t d = 1;; ++d) {
        idx_t cube = d * d * d;
        if (cube > max_sum) break;
        /* update DP: for each cube, count combinations using it any number of times */
        for (int used = 1; used <= max_used; ++used) {
            for (size_t sum = cube; sum <= max_sum; ++sum) {
                unsigned char add = DP(used-1, sum - cube);
                if (!add) continue;
                unsigned char current = DP(used, sum);
                unsigned int newval = current + add;
                DP(used, sum) = (newval > 4u ? 4 : (unsigned char)newval);
            }
        }
    }

    /* count how many sums have at least four representations */
    size_t needed = 0;
    for (size_t s = 1; s <= max_sum; ++s) {
        if (DP(max_used, s) >= 4u) ++needed;
    }

    /* allocate array for the terms */
    seq_buf = (seq_t *)malloc(needed * sizeof(seq_t));
    if (!seq_buf) { free(dp); DP(0,0) = 0; return; }
    seq_len = needed;

    /* fill the array */
    size_t idx = 0;
    for (size_t s = 1; s <= max_sum; ++s) {
        if (DP(max_used, s) >= 4u) {
            seq_buf[idx++] = (seq_t)s;
        }
    }

    free(dp);
    #undef DP
}

seq_t a(idx_t n)
{
    /* compute the list on first use */
    if (!seq_buf) compute_terms();

    /* out of bounds */
    if (n == 0 || n > seq_len) return (seq_t)0;
    return seq_buf[n - 1];
}
