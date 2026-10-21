/*
 * A244958  Smallest positive multiple of n whose base-7 representation
 *          contains only 0's and 1's.
 * https://oeis.org/A244958
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive multiple of n whose base‑7
 *            representation contains only 0's and 1's
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 95, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for n=0, but keep guard */

    idx_t start_rem = 1 % n;
    if (start_rem == 0) return (seq_t)1;

    /* allocate state arrays */
    int *parent   = (int*)malloc(n * sizeof(int));
    unsigned char *digit = (unsigned char*)malloc(n * sizeof(unsigned char));
    bool *visited = (bool*)malloc(n * sizeof(bool));
    idx_t *queue  = (idx_t*)malloc(n * sizeof(idx_t));
    if (!parent || !digit || !visited || !queue) {
        /* allocation failed – return 0 (unlikely in normal use) */
        free(parent); free(digit); free(visited); free(queue);
        return 0;
    }

    for (idx_t i = 0; i < n; ++i) visited[i] = false;

    /* BFS queue indices */
    idx_t front = 0, back = 0;

    /* start from remainder of 1 */
    visited[start_rem] = true;
    parent[start_rem] = -1;  /* sentinel for the root */
    digit[start_rem] = 1;    /* the first (and only) digit of the start node */
    queue[back++] = start_rem;

    bool found = false;
    while (front < back && !found) {
        idx_t cur = queue[front++];
        for (unsigned char d = 0; d <= 1; ++d) {
            idx_t newrem = (cur * 7 + d) % n;
            if (!visited[newrem]) {
                visited[newrem] = true;
                parent[newrem] = (int)cur;
                digit[newrem] = d;
                queue[back++] = newrem;
                if (newrem == 0) { found = true; break; }
            }
        }
    }

    /* reconstruct digits from remainder 0 back to start */
    /* first count length */
    idx_t rem = 0;
    idx_t len = 0;
    while (true) {
        len++;
        if (parent[rem] == -1) break;
        rem = parent[rem];
    }

    unsigned char *digits = (unsigned char*)malloc(len * sizeof(unsigned char));
    rem = 0; idx_t k = 0;
    while (true) {
        digits[k++] = digit[rem];
        if (parent[rem] == -1) break;
        rem = parent[rem];
    }

    /* compute the actual value in base 10 (or arbitrary-precision) */
    seq_t value = 0;
    for (idx_t i = len; i > 0; --i)
        value = value * 7 + digits[i - 1];

    /* clean up */
    free(parent); free(digit); free(visited); free(queue); free(digits);
    return value;
}
