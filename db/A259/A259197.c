/*
 * A259197  Number of partitions of n into seven primes.
 * https://oeis.org/A259197
 *
 * Offset:    14    (a(14) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [x^n y^7] ∏_{k≥1} 1/(1 - y·x^prime(k))
 * Range:     u8 n <= 70, u16 n <= 296, u32 n <= 3382, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /*  OEIS offset starts at 14 – for smaller n the sequence is undefined. */
    if (n < 14) return 0;

    /* -----------------------------------------------------------------
       Dynamic‑programming table:  dp[sum * 8 + c] = #ways to sum to `sum`
       using exactly `c` primes.  We keep only the last 8 columns
       (c = 0..7).  The table is stored as a 1‑dimensional array of
       length (n+1)·8 so that we can resize it with a single realloc.
       -----------------------------------------------------------------*/
    static seq_t *dp      = NULL;   /* stored as seq_t* */
    static idx_t dp_size  = 0;      /* current number of rows that are valid */
    const idx_t total_rows = n + 1;
    const size_t total_entries = (size_t)total_rows * 8;

    /* Allocate / enlarge the table if necessary.  (realloc returns void*,
       so a cast is required for C++.) */
    if (dp_size < total_rows) {
        seq_t *tmp = (seq_t *)realloc(dp, total_entries * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        dp = tmp;
        /* Zero all newly allocated cells – this also clears everything
           for the new size since we re‑zero the whole table for simplicity. */
        for (size_t i = 0; i < total_entries; ++i) dp[i] = (seq_t)0;
        dp_size = total_rows;
    } else {
        /* The table already has enough rows – just clear the used part. */
        for (size_t i = 0; i < total_entries; ++i) dp[i] = (seq_t)0;
    }

    /* -----------------------------------------------------------------
       Build a list of all primes ≤ n.  We use a simple incremental sieve
       that keeps a growing vector of primes.  The vector is reallocated
       only when the capacity is exceeded.  Since this list is rebuilt
       for every call, the overhead is negligible for n ≤ 10 000.
       -----------------------------------------------------------------*/
    static uint32_t *prime_buf = NULL;  /* vector of primes */
    static size_t prime_buf_cap = 0;
    size_t prime_cnt = 0;

    /* Ensure the buffer can hold at least n+1 primes – we over‑reserve
       by a factor of two to reduce reallocations. */
    if (prime_buf_cap < (size_t)(n + 1)) {
        size_t new_cap = (size_t)(n + 1) * 2;
        uint32_t *tmp = (uint32_t *)realloc(prime_buf, new_cap * sizeof(uint32_t));
        if (!tmp) return 0; /* allocation failure */
        prime_buf = tmp;
        prime_buf_cap = new_cap;
    }

    /* Populate the prime list in ascending order. */
    for (idx_t x = 2; x <= n; ++x) {
        bool is_prime = true;
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = prime_buf[j];
            if ((uint64_t)p * p > x) break;
            if (x % p == 0) { is_prime = false; break; }
        }
        if (is_prime) prime_buf[prime_cnt++] = (uint32_t)x;
    }

    /* -----------------------------------------------------------------
       DP: iterate over primes, updating for each possible sum.
       dp[sum * 8 + c] += dp[(sum - p) * 8 + (c - 1)]
       This counts unordered partitions because the outer loop over primes
       ensures that each multiset is considered only once.
       -----------------------------------------------------------------*/
    dp[0 * 8 + 0] = (seq_t)1;  /* one way to sum 0 with 0 primes */

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        uint32_t p = prime_buf[pi];
        for (idx_t sum = p; sum <= n; ++sum) {
            const idx_t base = sum * 8;
            const idx_t prev = (sum - p) * 8;
            for (idx_t c = 1; c <= 7; ++c) {
                dp[base + c] += dp[prev + (c - 1)];
            }
        }
    }

    return dp[n * 8 + 7];
}
