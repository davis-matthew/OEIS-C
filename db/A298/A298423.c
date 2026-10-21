/*
 * A298423  Number of integer partitions of n such that the predecessor of
 *          each part is divisible by the number of parts.
 * https://oeis.org/A298423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k | n} p(n/k - 1, k), where p(m,k) is the number of
 *            partitions of m into at most k parts
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

#define HASH_SIZE 262144            /* must be a power of two */
#define EMPTY_KEY ((uint64_t)-1)

/* Hash table for memoisation of p(m,k) values. */
static uint64_t hash_keys[HASH_SIZE];
static seq_t    hash_vals[HASH_SIZE];
static int      hash_inited = 0;

/* Recursive computation of p(m,k) with memoisation. */
static seq_t part(idx_t m, idx_t k)
{
    /* Base cases. */
    if (k == 0) return (m == 0) ? 1 : 0;
    if (m == 0) return 1;
    if (k > m) k = m;                     /* reduce to canonical form */

    uint64_t key = ((uint64_t)m << 32) | (uint64_t)k;

    if (!hash_inited) {
        /* Initialise the table with empty keys. */
        for (size_t i = 0; i < HASH_SIZE; ++i)
            hash_keys[i] = EMPTY_KEY;
        hash_inited = 1;
    }

    /* Probe for the key. */
    size_t pos = key & (HASH_SIZE - 1);
    while (hash_keys[pos] != EMPTY_KEY && hash_keys[pos] != key)
        pos = (pos + 1) & (HASH_SIZE - 1);

    if (hash_keys[pos] == key)            /* found in cache */
        return hash_vals[pos];

    /* Compute recursively and store. */
    seq_t val = part(m, k - 1) + part(m - k, k);

    hash_keys[pos] = key;
    hash_vals[pos] = val;
    return val;
}

/* Public function for sequence A298423. */
seq_t a(idx_t n)
{
    if (n == 0) return 1;

    seq_t sum = 0;
    idx_t d;

    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            idx_t k1 = d;
            idx_t M1 = n / k1 - 1;
            sum += part(M1, k1);

            idx_t k2 = n / d;
            if (k2 != k1) {
                idx_t M2 = n / k2 - 1;   /* equals d - 1 */
                sum += part(M2, k2);
            }
        }
    }

    return sum;
}
