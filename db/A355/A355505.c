/*
 * A355505  a(n) is the number of distinct cycles when iterating the function
 *          f_n(x), where f_n(x) is the sum of the digits in base n of x^2.
 * https://oeis.org/A355505
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 173, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum_base_n_sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A355505: the number of distinct cycles of the function
 * f_n(x) = sum of the base‑n digits of x², for a given base n ≥ 2. */
 
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum_base_n_sq.h"
 

 
seq_t a(idx_t n)
{
    if (n < 2) return 0;         /* offset is 2, nothing defined for n < 2 */
 
    /* A safe upper bound for any value that can arise in the dynamics.
     * For n ≥ 2 the maximum digit sum of (n²)² in base n is ≤ 5·(n‑1),
     * and the values further along the trajectory are still ≤ 6·(n‑1).
     * 6 is more than enough for all n up to 10 000. */
    size_t max_nodes = (size_t)(n - 1) * 6 + 10;
 
    uint8_t *state = (uint8_t*)calloc(max_nodes + 1, 1);
    /* state: 0 = unvisited, 1 = visiting (in current path), 2 = finished */
    size_t *stack = (size_t*)malloc((max_nodes + 1) * sizeof(size_t));
 
    seq_t cycle_count = 0;
 
    for (size_t start = 0; start <= max_nodes; ++start) {
        if (state[start] != 0) continue;     /* already processed */
 
        size_t stack_len = 0;
        size_t cur = start;
 
        while (1) {
            state[cur] = 1;
            stack[stack_len++] = cur;
 
            /* Compute the next value in the mapping. */
            seq_t next_val_seq = digit_sum_base_n_sq(n, cur);
            if (next_val_seq > max_nodes) {
                /* Next value is outside the considered range: no cycle involving
                 * nodes > max_nodes can return to cur.  Finish this path. */
                break;
            }
            size_t next = (size_t)next_val_seq;
 
            if (state[next] == 0) {
                cur = next;
                continue;
            } else if (state[next] == 1) {
                /* Found a new cycle.  The cycle consists of all nodes from
                 * 'next' to the current top of the stack. */
                size_t idx = 0;
                while (idx < stack_len && stack[idx] != next) idx++;
                if (idx < stack_len) {
                    ++cycle_count;
                }
            }
            break;
        }
 
        /* Mark all nodes on the current path as finished. */
        for (size_t i = 0; i < stack_len; ++i) {
            state[stack[i]] = 2;
        }
    }
 
    free(state);
    free(stack);
 
    return cycle_count;
}
