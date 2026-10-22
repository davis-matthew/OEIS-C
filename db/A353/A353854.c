/*
 * A353854  Length of the trajectory of the composition run-sum transformation
 *          (condensation) of the n-th composition in standard order.
 * https://oeis.org/A353854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) computed by simulating the run‑sum condensation
 *            transformation on the n-th composition in standard order
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Build the n‑th composition in standard order. */
    /* temp holds the gaps between successive 1‑positions in the
       binary expansion of n (reversed). */
    seq_t temp[128];
    size_t temp_len = 0;

    idx_t k = n;
    idx_t pos = 1;                /* 1‑based positions in the reversed bits */
    idx_t prev = 0;

    while (k > 0) {
        if (k & 1ULL) {
            temp[temp_len++] = (seq_t)(pos - prev);
            prev = pos;
        }
        k >>= 1;
        ++pos;
    }

    /* For n == 0 the composition is empty; its trajectory length is 1. */
    if (temp_len == 0)
        return 1;

    /* Reverse temp to obtain the composition in correct order. */
    seq_t comp[128];
    size_t comp_len = temp_len;
    for (size_t i = 0; i < comp_len; ++i)
        comp[i] = temp[comp_len - 1 - i];

    /* Compute the length of the run‑sum trajectory. */
    seq_t len = 1;
    seq_t cur[128];
    seq_t nxt[128];
    size_t cur_len = comp_len;
    for (size_t i = 0; i < cur_len; ++i)
        cur[i] = comp[i];

    while (true) {
        /* Run‑sum transformation. */
        size_t next_len = 0;
        seq_t run_sum = cur[0];
        seq_t run_val = cur[0];

        for (size_t i = 1; i < cur_len; ++i) {
            if (cur[i] == run_val) {
                run_sum += cur[i];
            } else {
                nxt[next_len++] = run_sum;
                run_val = cur[i];
                run_sum = cur[i];
            }
        }
        nxt[next_len++] = run_sum;   /* last run */

        /* Check if we have reached a fixed point. */
        bool same = (next_len == cur_len);
        if (same) {
            for (size_t i = 0; i < cur_len; ++i) {
                if (nxt[i] != cur[i]) {
                    same = false;
                    break;
                }
            }
        }
        if (same)
            break;

        /* Prepare for next iteration. */
        for (size_t i = 0; i < next_len; ++i)
            cur[i] = nxt[i];
        cur_len = next_len;
        ++len;
    }

    return len;
}
