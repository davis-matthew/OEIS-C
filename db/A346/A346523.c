/*
 * A346523  Number of sum pyramids for n.
 * https://oeis.org/A346523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of sum pyramids for n, counting only maximal pyramids
 * Range:     u8 n <= 40, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum possible height of a pyramid for n <= 10000 is well below 128. */
#define MAX_HEIGHT 128

/* Count maximal pyramids recursively.
   prev: previous row (length prev_len).
   used: array of length n+1 indicating which numbers are already used in the pyramid. */
static size_t count_from(const idx_t *prev, size_t prev_len, char *used, idx_t max_val)
{
    size_t total = 0;
    bool has_child = false;
    idx_t limit;
    if (prev_len == 1) {
        /* Apex row, choose a < b to avoid reflections. */
        limit = (prev[0] - 1) / 2;
    } else {
        limit = prev[0] - 1;
    }
    for (idx_t t = 1; t <= limit; ++t) {
        /* Build current row of length prev_len+1. */
        if (prev_len + 1 > MAX_HEIGHT)
            return 0; /* height too large, should not happen for n <= 10000 */
        idx_t cur[MAX_HEIGHT];
        cur[0] = t;
        bool ok = true;
        for (size_t k = 1; k <= prev_len; ++k) {
            cur[k] = prev[k - 1] - cur[k - 1];
            if (cur[k] == 0) { ok = false; break; }
            if (cur[k] > max_val) { ok = false; break; }
        }
        if (!ok) continue;
        /* Check distinctness against used and within the new row. */
        for (size_t i = 0; i <= prev_len; ++i) {
            if (used[cur[i]]) { ok = false; break; }
        }
        if (!ok) continue;
        for (size_t i = 0; i <= prev_len && ok; ++i) {
            for (size_t j = i + 1; j <= prev_len; ++j) {
                if (cur[i] == cur[j]) { ok = false; break; }
            }
        }
        if (!ok) continue;
        /* Accept this child row. */
        has_child = true;
        for (size_t i = 0; i <= prev_len; ++i)
            used[cur[i]] = 1;
        total += count_from(cur, prev_len + 1, used, max_val);
        for (size_t i = 0; i <= prev_len; ++i)
            used[cur[i]] = 0;
    }
    if (!has_child)
        return 1;   /* leaf pyramid (maximal) */
    return total;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    /* Allocate used array of size n+1. */
    size_t cap = (size_t)n + 1;
    char *used = (char *)calloc(cap, sizeof(char));
    if (!used)
        return (seq_t)0; /* allocation failure, unlikely for small n */
    /* Mark apex. */
    used[n] = 1;
    idx_t apex[1] = {n};
    size_t cnt = count_from(apex, 1, used, n);
    free(used);
    return (seq_t)cnt;
}
