/*
 * A304909  Expansion of x * (d/dx) Product_{k>=0} 1/(1 - x^(2^k)).
 * https://oeis.org/A304909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * p_2(n) where p_2(n) is the number of partitions of n
 *            into powers of 2
 * Range:     u8 n <= 12, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n * (sum_{k>=0} ... generating function) = n * (number of partitions of n into powers of 2). */

#include <stdlib.h>
#include <stdint.h>

/* Static storage for the partition counts p_2[n] */
static seq_t *p2_counts = NULL;
static idx_t p2_capacity = 0;

/* Ensure p_2 counts are available up to index n */
static void ensure_p2(idx_t n) {
    if (p2_capacity <= n) {
        /* Allocate or resize the array */
        if (p2_counts) {
            free(p2_counts);
        }
        p2_capacity = n + 1;
        p2_counts = (seq_t*)malloc(p2_capacity * sizeof(seq_t));
        if (!p2_counts) {
            /* In a real library you might handle this differently */
            exit(1);
        }
        /* Initialize */
        for (idx_t i = 0; i < p2_capacity; ++i) {
            p2_counts[i] = 0;
        }
        p2_counts[0] = 1;  /* empty partition */
        /* Coin‑change DP: coins are powers of two */
        for (idx_t power = 1; power <= n; power <<= 1) {
            for (idx_t sum = power; sum <= n; ++sum) {
                p2_counts[sum] += p2_counts[sum - power];
            }
        }
    }
}

/* External function required by the build */
seq_t a(idx_t n) {
    ensure_p2(n);
    return (seq_t)n * p2_counts[n];
}
