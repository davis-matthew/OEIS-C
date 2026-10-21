/*
 * A283550  Numbers that are not the sum of abundant numbers (not necessarily
 *          distinct).
 * https://oeis.org/A283550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 496, u32 n <= 496, u64 n <= 496,
 *            u128 n <= 496, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A283550: numbers that are not the sum of arbitrary many
 * abundant numbers (allowing repetitions).  The largest such number
 * is 991, so a(496)=991.  We sieve all proper divisor sums up to
 * 999, determine all abundant numbers, compute all reachable
 * sums using an unlimited‑coin change DP, and cache the list of
 * unreached numbers.  The function returns the nth element (1‑based)
 * of that list. */

#include <stdbool.h>
#include <stddef.h>

#define LIMIT 1000            /* > last term 991 */
#define NON_SUM_CAP 1000      /* ample space for 496 terms */

static bool initialized = false;
static seq_t  non_sum[NON_SUM_CAP];
static size_t non_sum_len = 0;

static bool reached[LIMIT];
static unsigned long long sigma[LIMIT];
static idx_t abundant[NON_SUM_CAP];
static size_t abundant_cnt = 0;

static void compute_cache(void)
{
    /* 1. Compute sums of proper divisors for 1 .. 999. */
    for (idx_t i = 1; i < LIMIT; ++i) {
        for (idx_t j = i << 1; j < LIMIT; j += i)
            sigma[j] += i;
    }

    /* 2. Collect abundant numbers up to 999. */
    for (idx_t i = 12; i < LIMIT; ++i) {        /* 12 is first abundant */
        if (sigma[i] > i)
            abundant[abundant_cnt++] = i;
    }

    /* 3. DP to find all sums of abundant numbers (unbounded). */
    for (idx_t i = 0; i < LIMIT; ++i) reached[i] = false;
    reached[0] = true;                         /* base case */

    for (size_t k = 0; k < abundant_cnt; ++k) {
        idx_t a = abundant[k];
        for (idx_t s = a; s < LIMIT; ++s) {
            if (reached[s - a]) reached[s] = true;
        }
    }

    /* 4. Build the list of numbers NOT reachable. */
    non_sum_len = 0;
    for (idx_t i = 1; i < LIMIT; ++i) {
        if (!reached[i]) {
            non_sum[non_sum_len++] = (seq_t)i;
        }
    }
}

seq_t a(idx_t n)
{
    if (!initialized) {
        compute_cache();
        initialized = true;
    }

    if (n == 0 || n > (idx_t)non_sum_len)
        return (seq_t)0;               /* out of bounds */

    return non_sum[(size_t)n - 1];
}
