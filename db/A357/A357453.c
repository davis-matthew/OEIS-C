/*
 * A357453  Number of compositions (ordered partitions) of n into tetranacci
 *          numbers 1,2,4,8,15,29, ... (A000078).
 * https://oeis.org/A357453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into tetranacci numbers
 *            (1,2,4,8,15,29,56,...) with a(0)=1
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>


seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *dp = NULL;          /* array of a(k) values */
    static idx_t dp_cap = 0;          /* capacity of dp */
    static idx_t dp_size = 0;         /* number of filled entries in dp */

    static seq_t *tetr = NULL;        /* list of tetranacci numbers used as parts */
    static idx_t tetr_cap = 0;        /* capacity of tetr */
    static idx_t tetr_count = 0;      /* number of tetr numbers stored */

    /* ---------- build tetranacci part list up to n ---------- */
    if (tetr_count == 0) {
        tetr_cap = 8;
        tetr = (seq_t*)malloc(sizeof(seq_t) * tetr_cap);
        tetr[0] = (seq_t)1;
        tetr[1] = (seq_t)2;
        tetr[2] = (seq_t)4;
        tetr[3] = (seq_t)8;
        tetr_count = 4;
    }
    while (tetr_count == 0 || tetr[tetr_count - 1] <= (seq_t)n) {
        if (tetr_count >= tetr_cap) {
            tetr_cap = tetr_cap ? tetr_cap * 2 : 8;
            tetr = (seq_t*)realloc(tetr, sizeof(seq_t) * tetr_cap);
        }
        /* next tetranacci = sum of the previous four */
        seq_t next = tetr[tetr_count - 1] + tetr[tetr_count - 2] +
                     tetr[tetr_count - 3] + tetr[tetr_count - 4];
        tetr[tetr_count++] = next;
    }

    /* ---------- extend dp array up to n ---------- */
    if (dp_cap < n + 1) {
        idx_t new_cap = n + 1;
        dp = (seq_t*)realloc(dp, sizeof(seq_t) * new_cap);
        dp_cap = new_cap;
    }

    /* compute dp values from current size up to n */
    for (idx_t i = dp_size; i <= n; ++i) {
        if (i == 0) {
            dp[0] = (seq_t)1;
            continue;
        }
        seq_t sum = 0;
        for (idx_t j = 0; j < tetr_count; ++j) {
            seq_t part = tetr[j];
            if (part > (seq_t)i) break;
            idx_t part_idx = (idx_t)part;            /* safe conversion, part <= i <= n */
            sum += dp[i - part_idx];
        }
        dp[i] = sum;
    }
    dp_size = n + 1;

    return dp[n];
}
