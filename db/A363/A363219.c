/*
 * A363219  Twice the median of the conjugate of the integer partition with
 *          Heinz number n.
 * https://oeis.org/A363219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * median(conjugate of the partition of n via Heinz
 *            number mapping)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    /* Factor n: n = Π p_i^e_i */
    uint64_t primes_buf[64];
    uint64_t exps_buf[64];
    size_t factor_cnt = 0;
    factor_u64((uint64_t)n, primes_buf, exps_buf, &factor_cnt);

    /* For n = 1 there are no primes -> result 0 */
    if (factor_cnt == 0)
        return (seq_t)0;

    /* Temporary arrays for part sizes (prime indices) and counts */
    size_t part_sizes[64];
    size_t part_counts[64];
    size_t i;

    size_t max_part_idx = 0;
    for (i = 0; i < factor_cnt; ++i) {
        /* prime_pi returns the number of primes <= p, i.e. the part size index */
        seq_t pi = prime_pi((idx_t)primes_buf[i]);
        size_t idx = (size_t)pi;
        part_sizes[i] = idx;
        part_counts[i] = (size_t)exps_buf[i];
        if (idx > max_part_idx)
            max_part_idx = idx;
    }

    /* Allocate frequency array for parts sizes */
    size_t *freq = (size_t *)calloc(max_part_idx + 1, sizeof(size_t));
    if (!freq)
        return (seq_t)0;   /* memory allocation failed, fallback to 0 */

    for (i = 0; i < factor_cnt; ++i)
        freq[part_sizes[i]] += part_counts[i];

    /* Compute conjugate: λ'_j = sum_{s=j}^{max_part_idx} freq[s] */
    for (size_t j = max_part_idx; j > 1; --j)
        freq[j - 1] += freq[j];

    /* The conjugate multiset has length L = max_part_idx */
    size_t L = max_part_idx;
    if (L == 0) {
        free(freq);
        return (seq_t)0;
    }

    seq_t median2;
    if (L & 1) {                     /* odd length */
        size_t m = L / 2 + 1;         /* 1‑based median position */
        median2 = (seq_t)freq[m] * 2;
    } else {                         /* even length */
        median2 = (seq_t)freq[L / 2] + (seq_t)freq[L / 2 + 1];
    }

    free(freq);
    return median2;
}
