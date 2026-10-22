/*
 * A326334  Number of sortable factorizations of n.
 * https://oeis.org/A326334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = number of sortable factorizations of n.
   The program enumerates all unordered factorizations of n (factors > 1)
   and verifies that the factor intervals [min prime, max prime] can be
   arranged in non‑decreasing order without overlap.  The result is
   cached per n. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static uint32_t *spf = NULL;          /* smallest prime factor table */
static size_t   spf_size = 0;         /* allocated size of spf */

static seq_t *memo = NULL;           /* memoised results */
static size_t memo_size = 0;          /* allocated size of memo */

/*------------------------------------------------------------*/
/* Simple sieve to fill spf[2..limit] with the smallest prime*/
static void precompute_spf(size_t limit)
{
    if (limit < 2) return;

    if (spf_size < limit + 1) {
        free(spf);
        spf = (uint32_t *)calloc(limit + 1, sizeof(uint32_t));
        spf_size = limit + 1;
    }

    for (size_t i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;

    for (size_t i = 2; i * i <= limit; ++i) {
        if (spf[i] == (uint32_t)i) {
            for (size_t j = i * i; j <= limit; j += i) {
                if (spf[j] == (uint32_t)j) spf[j] = (uint32_t)i;
            }
        }
    }

    /* Ensure memo table big enough for this limit */
    if (memo_size < limit + 1) {
        free(memo);
        memo = (seq_t *)calloc(limit + 1, sizeof(seq_t));
        memo_size = limit + 1;
    }
}

/*------------------------------------------------------------*/
/* Compute min and max prime factor of x (x > 1). */
static void get_minmax_prime(idx_t x, uint64_t *minp, uint64_t *maxp)
{
    *minp = UINT64_MAX;
    *maxp = 0;
    while (x > 1) {
        uint64_t p = spf[(size_t)x];
        if (p < *minp) *minp = p;
        if (p > *maxp) *maxp = p;
        do {
            x /= p;
        } while (x % p == 0);
    }
}

/*------------------------------------------------------------*/
/* Test whether a factorisation (stored in factors[0..len-1]) is sortable. */
static bool is_sortable(const idx_t *factors, size_t len)
{
    if (len <= 1) return true;

    /* Arrays to hold min and max prime of each factor */
    uint64_t mins[64];
    uint64_t maxs[64];

    for (size_t i = 0; i < len; ++i) {
        get_minmax_prime(factors[i], &mins[i], &maxs[i]);
    }

    /* Insertion sort by min ascending, then max ascending */
    for (size_t i = 1; i < len; ++i) {
        uint64_t mi = mins[i];
        uint64_t ma = maxs[i];
        size_t j = i;
        while (j > 0 && mins[j - 1] > mi) {
            mins[j] = mins[j - 1];
            maxs[j] = maxs[j - 1];
            --j;
        }
        while (j > 0 && mins[j - 1] == mi && maxs[j - 1] > ma) {
            mins[j] = mins[j - 1];
            maxs[j] = maxs[j - 1];
            --j;
        }
        mins[j] = mi;
        maxs[j] = ma;
    }

    /* Check that max of each interval <= min of the next */
    for (size_t i = 0; i + 1 < len; ++i) {
        if (maxs[i] > mins[i + 1]) return false;
    }
    return true;
}

/*------------------------------------------------------------*/
/* Recursive enumeration of unordered factorizations of 'remaining'.
   'min_factor' guarantees non-decreasing order of factors. */
static void dfs(idx_t remaining, idx_t min_factor,
                idx_t *factors, size_t depth, seq_t *cnt)
{
    if (remaining == 1) {
        if (is_sortable(factors, depth)) (*cnt)++;
        return;
    }

    /* Try all divisors d (>= min_factor) up to sqrt(remaining) */
    for (uint64_t d = min_factor; d * d <= remaining; ++d) {
        if (remaining % d == 0) {
            factors[depth] = d;
            dfs(remaining / d, d, factors, depth + 1, cnt);
        }
    }

    /* Include remaining itself as a factor if allowed */
    if (remaining >= min_factor) {
        factors[depth] = remaining;
        dfs(1, remaining, factors, depth + 1, cnt);
    }
}

/*------------------------------------------------------------*/
/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined entry, return 0 */
    if (n == 1) return 1;          /* a(1) = 1 */

    precompute_spf((size_t)n);

    /* Return cached value if already computed */
    if (memo[(size_t)n] != 0) return memo[(size_t)n];

    static idx_t factors[64];
    seq_t count = 0;
    dfs(n, 2, factors, 0, &count);

    memo[(size_t)n] = count;
    return count;
}
