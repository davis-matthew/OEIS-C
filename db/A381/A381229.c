/*
 * A381229  a(n) is the number of distinct positive integers that can be
 *          obtained by starting with n, and optionally applying the
 *          operations square root, floor, and ceiling, in any order.
 * https://oeis.org/A381229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A381229: number of distinct positive integers reachable from n
 * by repeatedly applying floor(sqrt(x)) or ceil(sqrt(x)) in any order.
 * The implementation performs a depth‑first search over the directed
 * graph whose vertices are the integers 1..n and whose edges are the
 * allowed operations.  The count of visited vertices is returned. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Integer square root of an unsigned 64‑bit number using binary search.
 * Returns ⌊√n⌋.  Result fits in idx_t. */
static inline idx_t isqrt_idx(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

/* Floor sqrt wrapper. */
static inline idx_t floor_sqrt_idx(idx_t n) { return isqrt_idx(n); }

/* Ceil sqrt wrapper; uses the result of floor_sqrt_idx. */
static inline idx_t ceil_sqrt_idx(idx_t n)
{
    idx_t f = floor_sqrt_idx(n);
    return ((unsigned __int128)f * f == n) ? f : f + 1;
}

/* Depth‑first traversal with explicit stack.  Returns the number of
 * distinct integers reachable from the starting value n. */
static seq_t count_set(idx_t n)
{
    if (n == 0)          /* sequence defined for n ≥ 1; protect anyway */
        return 0;

    size_t sz = (size_t)n + 1;
    bool *visited = (bool *)calloc(sz, sizeof(bool));
    if (!visited)            /* allocation failed */
        return 0;
    idx_t *stack = (idx_t *)malloc(sz * sizeof(idx_t));
    if (!stack) {
        free(visited);
        return 0;
    }

    size_t top = 0;
    size_t seen = 0;
    visited[n] = true;
    stack[top++] = n;

    while (top) {
        idx_t x = stack[--top];
        ++seen;

        if (x == 1)          /* 1 only loops to itself */
            continue;

        idx_t f = floor_sqrt_idx(x);
        if (!visited[f]) {
            visited[f] = true;
            stack[top++] = f;
        }

        idx_t c = ceil_sqrt_idx(x);
        if (c != f && !visited[c]) {
            visited[c] = true;
            stack[top++] = c;
        }
    }

    free(visited);
    free(stack);
    return (seq_t)seen;
}

seq_t a(idx_t n)
{
    return count_set(n);
}
