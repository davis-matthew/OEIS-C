/*
 * A257345  Regard the terms of A004290 as binary numbers and convert to base
 *          10.
 * https://oeis.org/A257345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A004290(n) read as a binary number; found by BFS over
 *            remainders mod n (digits 0/1), a(0)=0
 * Range:     u8 n <= 8, u16 n <= 98, u32 n <= 9998, u64 n <= 9998,
 *            u128 n <= 9998, big any n
 * Validated: standard, pass, 81 terms through n = 9786 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    bool *visited = (bool *)calloc((size_t)n, sizeof(bool));
    idx_t *parent = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    int *digit = (int *)malloc((size_t)n * sizeof(int));
    idx_t *queue = (idx_t *)malloc((size_t)n * sizeof(idx_t));

    for (idx_t i = 0; i < n; i++) {
        parent[i] = (idx_t)-1;
        digit[i] = -1;
    }

    idx_t front = 0, rear = 0;
    idx_t start_remainder = 1 % n;
    queue[rear++] = start_remainder;
    visited[start_remainder] = true;

    while (front < rear) {
        idx_t r = queue[front++];
        if (r == 0) break;

        idx_t r_next = (r * 10) % n;
        if (!visited[r_next]) {
            visited[r_next] = true;
            parent[r_next] = r;
            digit[r_next] = 0;
            queue[rear++] = r_next;
        }

        r_next = (r * 10 + 1) % n;
        if (!visited[r_next]) {
            visited[r_next] = true;
            parent[r_next] = r;
            digit[r_next] = 1;
            queue[rear++] = r_next;
        }
    }

    int *path = (int *)malloc((size_t)(n + 1) * sizeof(int));
    idx_t path_len = 0;

    for (idx_t r = 0; r != start_remainder; r = parent[r]) {
        path[path_len++] = digit[r];
    }
    path[path_len++] = 1;

    seq_t result = 0;
    for (idx_t i = path_len; i > 0; i--) {
        result = result * 2 + (seq_t)path[i - 1];
    }

    free(visited);
    free(parent);
    free(digit);
    free(queue);
    free(path);

    return result;
}
