/*
 * A337225  a(n) is the number of distinct integers k that can be obtained by
 *          starting from the binary expansion of n and repeatedly replacing
 *          some square XX by X.
 * https://oeis.org/A337225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A337225 – number of distinct integers reachable
   from n by repeatedly replacing a square XX by a single copy X.
   This implementation performs a DFS over the reduction graph,
   marking visited states with a byte array.  Complexity is
   O(number of reachable states) per call, which is at worst O(n). */

#include <stdint.h>
#include <stdlib.h>

/* Recursive helper: returns the number of distinct values reachable
   from x (including x itself).  'seen' marks values that have
   already been visited during this DFS. */
static seq_t dfs(idx_t x, uint8_t *seen)
{
    if (seen[x])           /* already counted */
        return 0;
    seen[x] = 1;           /* mark visited */
    seq_t count = 1;       /* count x itself */

    /* Prepare the binary representation of x. */
    int bits[64];
    int len;
    if (x == 0) {
        len = 1;
        bits[0] = 0;
    } else {
        unsigned long long tmp = (unsigned long long)x;
        len = 64 - __builtin_clzll(tmp);
        for (int i = 0; i < len; ++i)
            bits[i] = (int)((tmp >> (len - 1 - i)) & 1ULL);
    }

    /* Enumerate all adjacent equal substrings XX. */
    for (int i = 0; i < len; ++i) {
        for (int L = 1; i + 2 * L <= len; ++L) {
            int same = 1;
            for (int p = 0; p < L; ++p)
                if (bits[i + p] != bits[i + L + p]) { same = 0; break; }
            if (!same) continue;

            /* Build the child value by deleting the second copy. */
            idx_t child = 0;
            for (int pos = 0; pos < len; ++pos) {
                if (pos >= i + L && pos < i + 2 * L)
                    continue;               /* skip second copy */
                child = (child << 1) | (unsigned)bits[pos];
            }

            count += dfs(child, seen);
        }
    }
    return count;
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    /* Allocate a visited array: reachable values never exceed n. */
    uint8_t *seen = (uint8_t *)malloc((n + 1) * sizeof(uint8_t));
    for (idx_t i = 0; i <= n; ++i) seen[i] = 0;

    seq_t result = dfs(n, seen);
    free(seen);
    return result;
}
