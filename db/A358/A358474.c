/*
 * A358474  Number of 6's that appeared by n-th step when constructing
 *          A030717.
 * https://oeis.org/A358474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   simulate A030717 row by row. Row 1 = [1]; each later row lists
 *            the counts so far of each distinct value in the first list, in
 *            ascending order of value. a(n) = number of 6's in rows 1..n
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Validated: partial, pass, 82 terms through n = 82 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

seq_t a(idx_t n)
{
    size_t cap = 64;
    uint64_t *cnt = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *snap = (uint64_t *)malloc(cap * sizeof(uint64_t));
    size_t maxv = 1;
    seq_t sixes = 0;

    cnt[1] = 1; /* row 1 = [1] */

    for (idx_t step = 2; step <= n; ++step) {
        /* snapshot counts of the distinct values, in ascending order */
        size_t k = 0;
        for (size_t v = 1; v <= maxv; ++v)
            if (cnt[v] != 0) snap[k++] = cnt[v];

        /* append them to the first list */
        for (size_t i = 0; i < k; ++i) {
            uint64_t f = snap[i];
            if (f == 6) sixes++;
            if ((size_t)f >= cap) {
                size_t ncap = cap;
                while (ncap <= (size_t)f) ncap <<= 1;
                cnt = (uint64_t *)realloc(cnt, ncap * sizeof(uint64_t));
                memset(cnt + cap, 0, (ncap - cap) * sizeof(uint64_t));
                snap = (uint64_t *)realloc(snap, ncap * sizeof(uint64_t));
                cap = ncap;
            }
            cnt[f]++;
            if ((size_t)f > maxv) maxv = (size_t)f;
        }
    }

    free(cnt);
    free(snap);
    return sixes;
}
