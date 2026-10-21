/*
 * A295749  Numbers that have exactly eight representations of a sum of seven
 *          nonnegative squares.
 * https://oeis.org/A295749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A295749
   Numbers that have exactly eight unordered representations as a sum of
   seven non‑negative squares.
   The sequence is known to be finite and complete; all terms lie
   in the range 0 ≤ x ≤ 5408.  A single DP is performed once to
   enumerate every x with exactly eight representations, the results
   stored in ascending order.  a(idx_t n) returns the n‑th term
   (1‑based).  Values fit in seq_t; intermediates use 64‑bit
   unsigned integers. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are supplied by the build environment. */

static bool        _initialized = false;
static idx_t       _seq_len     = 0;
static seq_t      *_seq_arr     = NULL;

/* Builds the sequence once.  All values are <= 5408, so we pre‑compute
   representations for every integer in this range.  The DP counts
   unordered combinations of 7 squares (including 0). */
static void _build_sequence(void)
{
    const unsigned MAX_N = 5408;          /* proven upper bound   */
    const unsigned J     = 7;             /* exactly 7 squares   */

    /* dp[j][s] – number of unordered ways to write s with j squares */
    uint64_t *dp = (uint64_t *)calloc((J + 1) * (MAX_N + 1), sizeof(uint64_t));
    if (!dp) return;                      /* allocation failure – abort */
    uint64_t (*dp2)[MAX_N + 1] = (uint64_t (*)[MAX_N + 1])dp;

    dp2[0][0] = 1ULL;                     /* one way to make 0 with 0 squares */

    /* Iterate over all possible squared values (including 0). */
    for (unsigned k = 0; k * k <= MAX_N; ++k) {
        unsigned sq = k * k;
        for (unsigned j = 1; j <= J; ++j) {
            for (unsigned s = sq; s <= MAX_N; ++s) {
                dp2[j][s] += dp2[j - 1][s - sq];
            }
        }
    }

    /* Count how many integers satisfy the condition. */
    size_t count = 0;
    for (unsigned s = 0; s <= MAX_N; ++s) {
        if (dp2[7][s] == 8ULL) ++count;
    }

    if (count == 0) {      /* should never happen for this sequence */
        free(dp);
        return;
    }

    /* Allocate array for the sequence values. */
    seq_t *arr = (seq_t *)calloc(count, sizeof(seq_t));
    if (!arr) { free(dp); return; }

    /* Store the qualifying numbers in ascending order. */
    size_t pos = 0;
    for (unsigned s = 0; s <= MAX_N; ++s) {
        if (dp2[7][s] == 8ULL) {
            arr[pos++] = (seq_t)s;
        }
    }

    _seq_arr = arr;
    _seq_len = count;
    _initialized = true;
    free(dp);
}

seq_t a(idx_t n)
{
    if (!_initialized) _build_sequence();

    if (n < 1 || n > _seq_len) return 0;  /* undefined index → 0 */
    return _seq_arr[n - 1];
}
