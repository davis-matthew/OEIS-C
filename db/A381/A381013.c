/*
 * A381013  If n = Product (p_j^k_j) then a(n) = Product partition(p_j^k_j).
 * https://oeis.org/A381013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏ partition(p_j^k_j) where n = ∏ p_j^k_j
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 127, u64 n <= 418,
 *            u128 n <= 1458, big any n
 * Validated: standard, pass, 76 terms through n = 9623 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Factorises n and stores its prime powers into out_powers[].
   Returns the number of prime powers stored. */
static size_t factor_prime_powers(idx_t n, idx_t out_powers[64])
{
    size_t cnt = 0;
    idx_t  temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
        if (temp % p == 0) {
            idx_t pow = 1;
            while (temp % p == 0) {
                temp /= p;
                pow *= p;
            }
            out_powers[cnt++] = pow;
        }

    if (temp > 1)        /* remaining prime factor > sqrt(original n) */
        out_powers[cnt++] = temp;

    return cnt;
}

/* Cached partition numbers up to the current maximum prime‑power */
static seq_t *partition_table = NULL;
static idx_t  partition_capacity = 0;

/* Recompute all partition numbers up to max_needed (inclusive) */
static void recompute_partitions(idx_t max_needed)
{
    if (partition_table)
        mem_free(partition_table);

    partition_capacity = max_needed + 1;
    partition_table = mem_new(seq_t, partition_capacity);
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
