/*
 * A267592  Number of OFF (white) cells in the n-th iteration of the "Rule
 *          169" elementary cellular automaton starting with a single ON
 *          (black) cell.
 * https://oeis.org/A267592
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Simulates Rule 169 elementary cellular automaton starting with a single ON cell
   and counts the number of OFF cells after n iterations.
   The algorithm keeps only the finite set of OFF positions; all other cells
   are assumed to be ON.  The OFF set is updated by direct examination of the
   local neighbourhood according to rule 169. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* binary search in a sorted array of long long */
static inline bool contains(const long long *arr, size_t len, long long key)
{
    size_t lo = 0, hi = len;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (arr[mid] < key)
            lo = mid + 1;
        else
            hi = mid;
    }
    return (lo < len && arr[lo] == key);
}

seq_t a(idx_t n)
{
    /* n = 0 : only one ON cell, so no interior OFF cells */
    if (n == 0)
        return 0;

    /* n = 1 : after the first step the three cells –1,0,1 are OFF */
    if (n == 1)
        return 3;

    /* OFF set for step 1 */
    size_t cur_len = 3;
    long long *cur = (long long*)malloc(cur_len * sizeof(long long));
    if (!cur)
        return 0;
    cur[0] = -1; cur[1] = 0; cur[2] = 1;

    for (idx_t step = 2; step <= n; ++step) {
        /* neighbouring range that can change */
        long long minOff = cur[0];
        long long maxOff = cur[cur_len - 1];
        long long left  = minOff - 1;
        long long right = maxOff + 1;

        size_t est = (size_t)(right - left + 1);
        long long *nxt = (long long*)malloc(est * sizeof(long long));
        if (!nxt) {
            free(cur);
            return 0;
        }
        size_t nxt_len = 0;

        for (long long j = left; j <= right; ++j) {
            bool leftOff   = contains(cur, cur_len, j - 1);
            bool centerOff = contains(cur, cur_len, j);
            bool rightOff  = contains(cur, cur_len, j + 1);

            /* 1 = ON, 0 = OFF ; pattern = 4*left + 2*center + right */
            int pat = (!leftOff << 2) | (!centerOff << 1) | (!rightOff);

            /* rule 169 produces 0 for patterns 001,010,100,110 (1,2,4,6) */
            if (pat == 1 || pat == 2 || pat == 4 || pat == 6)
                nxt[nxt_len++] = j;
        }

        free(cur);
        cur = nxt;
        cur_len = nxt_len;
    }

    seq_t result = (seq_t)cur_len;  /* number of OFF cells at step n */
    free(cur);
    return result;
}
