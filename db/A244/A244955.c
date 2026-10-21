/*
 * A244955  Smallest positive multiple of n whose base-4 representation
 *          contains only 0's and 1's.
 * https://oeis.org/A244955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest positive multiple of n whose base‑4 *
 *            representation contains only the digits 0 and 1
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul4_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mul4_mod.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    idx_t N = n;

    /* BFS structures. */
    idx_t *queue = (idx_t *)malloc(N * sizeof(idx_t));
    bool *vis   = (bool *)calloc(N, sizeof(bool));
    idx_t *parent   = (idx_t *)malloc(N * sizeof(idx_t));
    uint8_t *edge_digit = (uint8_t *)malloc(N * sizeof(uint8_t));

    if (!queue || !vis || !parent || !edge_digit) {
        free(queue); free(vis); free(parent); free(edge_digit);
        return (seq_t)0;
    }

    /* Initialise search with the first digit '1'. */
    idx_t start_rem = 1 % N;
    idx_t head = 0, tail = 0;
    queue[tail++] = start_rem;
    vis[start_rem] = true;
    parent[start_rem] = start_rem;    /* root has no parent */
    edge_digit[start_rem] = 0;        /* unused */

    /* BFS to find remainder 0. */
    while (head < tail) {
        idx_t r = queue[head++];

        idx_t r0 = mul4_mod(r, N);
        if (!vis[r0]) {
            vis[r0] = true;
            parent[r0] = r;
            edge_digit[r0] = 0;
            if (r0 == 0) break;
            queue[tail++] = r0;
        }

        idx_t r1 = (r0 + 1) % N;
        if (!vis[r1]) {
            vis[r1] = true;
            parent[r1] = r;
            edge_digit[r1] = 1;
            if (r1 == 0) break;
            queue[tail++] = r1;
        }
    }

    /* Release BFS arrays that are no longer needed. */
    free(queue);
    free(vis);

    /* Build the digit sequence from remainder 0 back to start. */
    uint8_t *rev = (uint8_t *)malloc(N * sizeof(uint8_t));
    size_t rev_len = 0;
    idx_t cur = 0;
    while (cur != start_rem) {
        rev[rev_len++] = edge_digit[cur];
        cur = parent[cur];
    }

    /* Construct the value in base 4.  Leading digit '1' is already
     * represented by the start state. */
    seq_t value = (seq_t)1;
    for (size_t i = 0; i < rev_len; ++i) {
        uint8_t d = rev[rev_len - 1 - i];  /* most significant first */
        value = value * (seq_t)4 + (seq_t)d;
    }

    free(parent);
    free(edge_digit);
    free(rev);

    return value;
}
