/*
 * A244959  Smallest positive multiple of n whose base 8 representation
 *          contains only 0's and 1's.
 * https://oeis.org/A244959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 62, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 961 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest positive multiple of n whose base‑8 representation
 * contains only 0's and 1's by a breadth‑first search over remainders
 * modulo n.  The algorithm guarantees the minimal such number for every n ≥ 1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;

    /* ensure cache large enough, initialise new entries to 0 */
    size_t old_cap = cache_cap;
    ensure_cache(n, &cache, &cache_cap);
    for (size_t i = old_cap; i < cache_cap; ++i)
        cache[i] = (seq_t)0;

    if (n < 1) return (seq_t)0;          /* undefined for n < 1 */

    if (cache[n-1] != (seq_t)0)          /* already computed */
        return cache[n-1];

    /* Special case n == 1 */
    if (n == 1) {
        cache[0] = (seq_t)1;
        return cache[0];
    }

    /* BFS over remainders modulo n */
    int *queue = (int*)malloc(n * sizeof(int));
    bool *visited = (bool*)malloc(n * sizeof(bool));
    int  *parent = (int*)malloc(n * sizeof(int));
    unsigned char *dig = (unsigned char*)malloc(n * sizeof(unsigned char));

    for (int i = 0; i < (int)n; ++i) {
        visited[i] = false;
        parent[i] = -1;
    }

    int front = 0, back = 0;
    int start = 1 % (int)n;                /* remainder of the number "1" */
    visited[start] = true;
    parent[start] = -1;
    dig[start] = 1;
    queue[back++] = start;

    if (start == 0) {                      /* n divides 1 */
        free(queue); free(visited); free(parent); free(dig);
        cache[n-1] = (seq_t)1;
        return cache[n-1];
    }

    while (front < back) {
        int r = queue[front++];
        for (int d = 0; d <= 1; ++d) {
            int newr = ((r * 8) + d) % (int)n;
            if (!visited[newr]) {
                visited[newr] = true;
                parent[newr] = r;
                dig[newr]   = (unsigned char)d;

                if (newr == 0) {            /* found minimal number */
                    /* reconstruct digits from root to leaf */
                    unsigned char *stack = (unsigned char*)malloc(n + 5);
                    int stacklen = 0;
                    int cur = newr;
                    while (cur != -1) {
                        stack[stacklen++] = dig[cur];
                        cur = parent[cur];
                    }
                    /* convert stack (LSB→MSB) to seq_t value */
                    seq_t result = 0;
                    for (int i = stacklen - 1; i >= 0; --i)
                        result = result * (seq_t)8 + (seq_t)stack[i];
                    free(stack);
                    free(queue); free(visited); free(parent); free(dig);
                    cache[n-1] = result;
                    return result;
                }
                queue[back++] = newr;
            }
        }
    }

    /* Should never reach here */
    free(queue); free(visited); free(parent); free(dig);
    return (seq_t)0;
}
