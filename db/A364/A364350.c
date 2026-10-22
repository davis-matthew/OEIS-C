/*
 * A364350  Number of strict integer partitions of n such that no part can be
 *          written as a nonnegative linear combination of the others.
 * https://oeis.org/A364350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of strict integer partitions of n such that no part can
 *            be written as a nonnegative linear combination of the others
 * Range:     u8 n <= 59, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   bit_clear
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a backtracking search that maintains, for the
   current partial partition, the set of sums that can be formed from the
   already chosen parts.  A new part is allowed only if it is not already
   representable.  The reachable sums are stored as a bitset. */
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/bit_clear.h"

static inline int  bit_get(const uint64_t *reach, int idx) {
    return (reach[idx >> 6] >> (idx & 63)) & 1ULL;
}
static inline void bit_set(uint64_t *reach, int idx) {
    reach[idx >> 6] |= 1ULL << (idx & 63);
}

/* Recursive backtracking: min_val is the smallest value that may be used
   next; remaining is the sum still to be filled. */
static void dfs(int min_val, int remaining, int N,
                uint64_t *reach, int *stack, int *stackTop, seq_t *count)
{
    if (remaining == 0) {
        ++(*count);
        return;
    }
    for (int val = min_val; val <= remaining; ++val) {
        if (bit_get(reach, val))
            continue;              /* val is already representable */
        int oldTop = *stackTop;
        /* add val to the set: update reachable sums */
        for (int s = val; s <= N; ++s) {
            if (bit_get(reach, s - val)) {
                if (!bit_get(reach, s)) {
                    bit_set(reach, s);
                    stack[(*stackTop)++] = s;   /* remember changes */
                }
            }
        }
        dfs(val + 1, remaining - val, N, reach, stack, stackTop, count);
        /* undo the changes made when adding val */
        while (*stackTop > oldTop) {
            int pos = stack[--(*stackTop)];
            bit_clear(reach, pos);
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;   /* empty partition */

    int N = (int)n;                      /* n fits in int (≤ 10000) */
    int words = (N + 1 + 63) >> 6;       /* number of 64‑bit words */
    uint64_t *reach = (uint64_t *)malloc(sizeof(uint64_t) * words);
    memset(reach, 0, sizeof(uint64_t) * words);

    int *stack = (int *)malloc(sizeof(int) * (N + 1));
    int stackTop = 0;
    seq_t count = 0;

    bit_set(reach, 0);   /* 0 is always representable */

    dfs(1, N, N, reach, stack, &stackTop, &count);

    free(stack);
    free(reach);
    return count;
}
