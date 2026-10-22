/*
 * A370571  Smallest multiple of n that when written in base 10 uses only 0's
 *          and 1's and at least one of each.
 * https://oeis.org/A370571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest positive multiple of n whose decimal
 *            representation uses only digits 0 and 1, containing at least one
 *            of each. This implementation performs a breadth‑first search
 *            over remainders modulo n
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 35, u64 n <= 791,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0; /* undefined, but guard anyway */

    /* number of possible states: remainder (0..n-1) × has0 (2) × has1 (2) */
    size_t visited_size = n * 4;
    size_t *visited = (size_t *)malloc(visited_size * sizeof(size_t));
    if (!visited) return (seq_t)0; /* allocation failed */
    const size_t VISITED_UNVISITED = SIZE_MAX;
    for (size_t i = 0; i < visited_size; ++i) visited[i] = VISITED_UNVISITED;

    /* Node representation for BFS */
    typedef struct {
        idx_t rem;
        bool has0;
        bool has1;
        size_t parent;   /* index of parent node, or VISITED_UNVISITED for root */
        char digit;      /* digit appended to reach this node */
    } Node;

    /* Preallocate arrays for nodes and queue */
    size_t capacity = visited_size;
    Node *nodes = (Node *)malloc(capacity * sizeof(Node));
    if (!nodes) { free(visited); return (seq_t)0; }
    size_t *queue = (size_t *)malloc(visited_size * sizeof(size_t));
    if (!queue) { free(visited); free(nodes); return (seq_t)0; }

    /* initial state: number "1" */
    Node start;
    start.rem = (idx_t)(1 % n);
    start.has0 = false;
    start.has1 = true;
    start.parent = VISITED_UNVISITED;
    start.digit = '1';

    size_t nodes_cnt = 1;
    nodes[0] = start;
    size_t key0 = ((size_t)start.rem << 2) | 0 | 1;  /* has0=0, has1=1 */
    visited[key0] = 0;
    size_t qhead = 0, qtail = 0;
    queue[qtail++] = 0;

    while (qhead < qtail) {
        size_t cur_idx = queue[qhead++];
        Node cur = nodes[cur_idx];

        /* check for solution */
        if (cur.rem == 0 && cur.has0 && cur.has1) {
            /* reconstruct the digit string in reverse order */
            size_t buf_cap = nodes_cnt + 1;
            char *buf = (char *)malloc(buf_cap);
            if (!buf) { free(visited); free(nodes); free(queue); return (seq_t)0; }
            size_t pos = 0;
            size_t walk = cur_idx;
            while (walk != VISITED_UNVISITED) {
                Node wnode = nodes[walk];
                buf[pos++] = wnode.digit;
                walk = wnode.parent;
            }
            /* reverse to obtain correct order */
            for (size_t i = 0; i < pos / 2; ++i) {
                char t = buf[i];
                buf[i] = buf[pos - 1 - i];
                buf[pos - 1 - i] = t;
            }
            buf[pos] = '\0';

            seq_t result = seq_from_str(buf);

            free(buf);
            free(visited);
            free(nodes);
            free(queue);
            return result;
        }

        /* expand with digits 0 and 1 */
        for (int d = 0; d <= 1; ++d) {
            idx_t new_rem = (cur.rem * 10 + d) % n;
            bool new_has0 = cur.has0 || (d == 0);
            bool new_has1 = cur.has1 || (d == 1);
            size_t new_key = ((size_t)new_rem << 2) | (new_has0 ? 2 : 0) | (new_has1 ? 1 : 0);
            if (visited[new_key] == VISITED_UNVISITED) {
                /* add new node */
                nodes[nodes_cnt].rem = new_rem;
                nodes[nodes_cnt].has0 = new_has0;
                nodes[nodes_cnt].has1 = new_has1;
                nodes[nodes_cnt].parent = cur_idx;
                nodes[nodes_cnt].digit = '0' + d;
                visited[new_key] = nodes_cnt;
                queue[qtail++] = nodes_cnt;
                ++nodes_cnt;
            }
        }
    }

    /* Theoretically unreachable: every n has a solution. */
    free(visited);
    free(nodes);
    free(queue);
    return (seq_t)0;
}
