/*
 * A381013  If n = Product (p_j^k_j) then a(n) = Product partition(p_j^k_j).
 * https://oeis.org/A381013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏ partition(p_j^k_j) where n = ∏ p_j^k_j
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 127, u64 n <= 418,
 *            u128 n <= 1458, big any n
 * Helpers:   factor_prime_powers
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_prime_powers.h"

/* Cached partition numbers up to the current maximum prime‑power */
static seq_t *partition_table = NULL;
static idx_t  partition_capacity = 0;

/* Recompute all partition numbers up to max_needed (inclusive) */
static void recompute_partitions(idx_t max_needed)
{
    if (partition_table)
        free(partition_table);

    partition_capacity = max_needed + 1;
    partition_table = (seq_t *)calloc(partition_capacity, sizeof(seq_t));
    if (!partition_table)
        exit(1);

    partition_table[0] = 1;
    for (idx_t coin = 1; coin <= max_needed; ++coin)
        for (idx_t sum = coin; sum <= max_needed; ++sum)
            partition_table[sum] += partition_table[sum - coin];
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;                 /* a(1) = 1, offset 1 */

    idx_t  prime_powers[64];
    size_t pc = factor_prime_powers(n, prime_powers);

    /* Determine the maximum prime power needed for the partition cache */
    idx_t max_needed = 0;
    for (size_t i = 0; i < pc; ++i)
        if (prime_powers[i] > max_needed)
            max_needed = prime_powers[i];

    if (max_needed >= partition_capacity)
        recompute_partitions(max_needed);

    seq_t result = 1;
    for (size_t i = 0; i < pc; ++i)
        result *= partition_table[prime_powers[i]];

    return result;
}
