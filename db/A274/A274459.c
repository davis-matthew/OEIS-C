/*
 * A274459  Least number of perfect powers that add up to n.
 * https://oeis.org/A274459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 177, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of A274459 – Least number of perfect powers that add up to n.
   The function uses dynamic programming and memoised generation of all perfect
   powers up to the requested n.  It is valid C11 and C++17.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static seq_t *powers = NULL;          /* sorted unique perfect powers up to limit */
static size_t   powers_count = 0;
static idx_t    max_power_generated = 0;

#if defined(__cplusplus)
    /* allocation of seq_t arrays in C++ */
    static inline seq_t *alloc_seq_array(size_t len)
    {
        return new seq_t[len];
    }
    static inline void free_seq_array(seq_t *p, size_t len)
    {
        (void)len;     /* length not needed for delete[] */
        delete[] p;
    }
#else
    /* allocation of seq_t arrays in C */
    static inline seq_t *alloc_seq_array(size_t len)
    {
        return (seq_t *)calloc(len, sizeof(seq_t));
    }
    static inline void free_seq_array(seq_t *p, size_t len)
    {
        (void)len;
        free(p);
    }
#endif

/* Ensure that all perfect powers <= limit are generated and stored in powers[]. */
static void ensure_powers(idx_t limit)
{
    if (limit <= max_power_generated)
        return;

    /* Use a bitmap to mark perfect powers. */
    uint8_t *seen = (uint8_t *)calloc((size_t)(limit + 1), 1);
    if (!seen)
        exit(EXIT_FAILURE);

    /* 1 is a perfect power (1^k for any k≥2). */
    seen[1] = 1;

    for (idx_t base = 2; base * base <= limit; ++base) {
        seq_t val = (seq_t)base * base;   /* base^2 */
        while (val <= limit) {
            seen[(size_t)val] = 1;
            if (val > limit / base)
                break;
            val *= base;                   /* base^(exp+1) */
        }
    }

    /* Count the perfect powers found. */
    size_t cnt = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (seen[i])
            ++cnt;

    /* Allocate array for the perfect powers. */
    seq_t *newpowers = alloc_seq_array(cnt);
    if (!newpowers) {
        free(seen);
        exit(EXIT_FAILURE);
    }

    /* Fill the array with sorted perfect powers. */
    size_t pos = 0;
    for (idx_t i = 1; i <= limit; ++i)
        if (seen[i])
            newpowers[pos++] = (seq_t)i;

    /* Replace old data. */
    free_seq_array(powers, powers_count);
    powers = newpowers;
    powers_count = cnt;
    max_power_generated = limit;

    free(seen);
}

/* Least number of perfect powers that sum to n. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* not used, safety */

    /* Make sure perfect powers up to n are ready. */
    ensure_powers(n);

    /* Dynamic programming array for solutions 0..n. */
    seq_t *dp = alloc_seq_array((size_t)(n + 1));
    if (!dp)
        exit(EXIT_FAILURE);

    dp[0] = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t best = (seq_t)(n + 1);   /* upper bound > any real answer */
        for (size_t j = 0; j < powers_count && (idx_t)powers[j] <= i; ++j) {
            idx_t rem = i - (idx_t)powers[j];
            seq_t cand = dp[rem] + 1;
            if (cand < best)
                best = cand;
        }
        dp[i] = best;
    }

    seq_t result = dp[n];
    free_seq_array(dp, n + 1);
    return result;
}
