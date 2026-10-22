/*
 * A339839  Number of factorizations of n into distinct primes or semiprimes.
 * https://oeis.org/A339839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n into distinct
 *            primes or semiprimes. Computation uses a 0‑1 DP over all primes
 *            and semiprimes ≤ n
 * Range:     u8 n <= 9693, u16 n <= 9693, u32 n <= 9693, u64 n <= 9693,
 *            u128 n <= 9693, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *dp = NULL;                 /* DP table up to current max n */
static idx_t max_n_computed = 0;         /* Largest n for which dp was built */

/* Build DP table up to the specified value of n */
static void build_tables(idx_t n)
{
    if (n <= max_n_computed) return;

    /* 1. Determine all primes up to n with a simple sieve */
    unsigned char *is_prime = (unsigned char*)calloc(n + 1, 1);
    if (!is_prime) return;                 /* allocation failure – leave dp unchanged */
    for (idx_t i = 2; i <= n; ++i) is_prime[i] = 1;
    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= n; j += p) {
                is_prime[j] = 0;
            }
        }
    }

    /* 2. Mark primes and semiprimes as allowed factors */
    unsigned char *is_factor = (unsigned char*)calloc(n + 1, 1);
    for (idx_t i = 2; i <= n; ++i) {
        if (is_prime[i]) is_factor[i] = 1;          /* prime */
    }

    /* semiprimes: product of two primes (allow equal primes) */
    for (idx_t i = 2; i <= n; ++i) {
        if (!is_prime[i]) continue;
        for (idx_t j = i; j <= n; ++j) {
            if (!is_prime[j]) continue;
            uint64_t prod = (uint64_t)i * j;
            if (prod > n) break;
            is_factor[(idx_t)prod] = 1;
        }
    }

    /* 3. Build a compact list of all allowed factors */
    size_t factor_cnt = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_factor[i]) ++factor_cnt;
    idx_t *factors = (idx_t*)malloc(factor_cnt * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= n; ++i) if (is_factor[i]) factors[pos++] = i;

    /* 4. Allocate and initialise DP table */
    seq_t *newdp = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; ++i) newdp[i] = 0;
    newdp[1] = 1;                 /* empty factorisation of 1 */

    /* 5. 0‑1 DP over the allowed factors */
    for (size_t fi = 0; fi < factor_cnt; ++fi) {
        idx_t f = factors[fi];
        for (idx_t m = n; m >= f; --m) {
            if (m % f == 0) {
                newdp[m] += newdp[m / f];
            }
        }
    }

    /* 6. Store the new table and clean up */
    free(is_factor);
    free(is_prime);
    free(factors);
    if (dp) free(dp);
    dp = newdp;
    max_n_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n = 0 */
    build_tables(n);
    return dp[n];
}
