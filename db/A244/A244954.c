/*
 * A244954  Smallest positive multiple of n whose base-3 representation
 *          contains only 0's and 1's.
 * https://oeis.org/A244954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 321, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = smallest positive multiple of n whose base‑3 
 * representation consists only of the digits 0 and 1.
 * The function computes this value for any n ≥ 1.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    /* All arrays are sized at n+1 to store indices 0 … n-1. */
    size_t len = (size_t)n + 1;
    bool *visited = (bool*)calloc(len, sizeof(bool));
    idx_t *parent  = (idx_t*)malloc(len * sizeof(idx_t));
    uint8_t *digit = (uint8_t*)malloc(len * sizeof(uint8_t));
    idx_t *queue   = (idx_t*)malloc(len * sizeof(idx_t));
    if (!visited || !parent || !digit || !queue) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(visited); free(parent); free(digit); free(queue);
        return (seq_t)0;
    }

    /* Initialise BFS */
    size_t head = 0, tail = 0;
    idx_t start = 1 % n;          /* first digit is '1' */
    visited[start] = true;
    parent[start]  = (idx_t)-1;    /* root has no parent */
    digit[start]   = 1;           /* leading 1 */
    queue[tail++]  = start;

    /* Target remainder is 0. */
    bool found = false;

    while (head < tail && !found) {
        idx_t cur = queue[head++];
        /* Expand by appending digits 0 then 1 (0 first gives smaller numbers). */
        for (int d = 0; d <= 1; ++d) {
            idx_t nxt = (cur * 3 + d) % n;
            if (!visited[nxt]) {
                visited[nxt] = true;
                parent[nxt]  = cur;
                digit[nxt]   = (uint8_t)d;
                queue[tail++] = nxt;
                if (nxt == 0) {
                    found = true;
                    break;
                }
            }
        }
    }

    /* Back‑track from remainder 0 to reconstruct the base‑3 digits. */
    int *digits = (int*)malloc(len * sizeof(int));
    if (!digits) {
        free(visited); free(parent); free(digit); free(queue);
        return (seq_t)0;
    }
    size_t cnt = 0;
    idx_t rem = 0;
    /* Follow parent pointers until the root. */
    while (rem != start) {
        digits[cnt++] = digit[rem];
        rem = parent[rem];
    }
    digits[cnt++] = digit[start];   /* leading digit */

    /* Convert the digit sequence to a value in seq_t. */
    seq_t result = 0;
    for (int i = (int)cnt - 1; i >= 0; --i) {
        result = result * 3 + digits[i];
    }

    /* Clean up. */
    free(visited);
    free(parent);
    free(digit);
    free(queue);
    free(digits);

    return result;
}
