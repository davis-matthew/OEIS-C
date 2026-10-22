/*
 * A343722  a(n) is the number of starting residues r modulo n from which
 *          repeated iterations of the mapping r -> r^2 mod n never reach a
 *          fixed point.
 * https://oeis.org/A343722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of residues r modulo n whose orbit under r -> r^2
 *            mod n never reaches a fixed point (i.e., r(r-1) ≡ 0 (mod n))
 * Range:     u8 n <= 261, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* helper to test whether r is a fixed point modulo n */
static inline bool is_fixed(idx_t r, idx_t n)
{
    if (r == 0 || r == 1)
        return true;
    unsigned long long prod = (unsigned long long)r * (unsigned long long)(r - 1);
    return (prod % (unsigned long long)n) == 0;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not defined for n=0 */

    const size_t N = (size_t)n;
    unsigned char *state = (unsigned char *)malloc(N);
    unsigned char *reach = (unsigned char *)malloc(N);
    idx_t *path = (idx_t *)malloc(N * sizeof(idx_t));
    if (!state || !reach || !path) {
        free(state);
        free(reach);
        free(path);
        return (seq_t)0;
    }

    /* initialize */
    for (size_t i = 0; i < N; ++i) {
        state[i] = 0;          /* 0 = unvisited, 1 = in stack, 2 = processed */
        reach[i] = 0;          /* 0 = never reaches a fixed point, 1 = reaches */
    }

    for (idx_t start = 0; start < n; ++start) {
        if (state[(size_t)start] != 0)
            continue;

        /* traverse component starting from start */
        size_t len = 0;
        idx_t cur = start;
        while (state[(size_t)cur] == 0) {
            state[(size_t)cur] = 1;          /* mark in current path */
            path[len++] = cur;
            unsigned long long prod = (unsigned long long)cur * (unsigned long long)cur;
            cur = (idx_t)(prod % (unsigned long long)n);
        }

        bool hasFixed;
        if (state[(size_t)cur] == 1) {
            /* a cycle was found; locate the first appearance of cur in path */
            size_t cycle_start = 0;
            while (path[cycle_start] != cur)
                ++cycle_start;

            /* check if any node on the cycle is a fixed point */
            hasFixed = false;
            for (size_t j = cycle_start; j < len; ++j) {
                if (is_fixed(path[j], n)) {
                    hasFixed = true;
                    break;
                }
            }
        } else {
            /* cur already belongs to a processed component */
            hasFixed = (reach[(size_t)cur] != 0);
        }

        /* mark all nodes in the component */
        for (size_t i = 0; i < len; ++i) {
            reach[(size_t)path[i]] = hasFixed ? 1 : 0;
            state[(size_t)path[i]] = 2;       /* component finished */
        }
    }

    /* count residues that never reach a fixed point */
    seq_t cnt = 0;
    for (size_t i = 0; i < N; ++i)
        if (reach[i] == 0)
            ++cnt;

    free(state);
    free(reach);
    free(path);
    return cnt;
}
