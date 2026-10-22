/*
 * A328855  Number of factorizations of n into numbers with an odd number of
 *          distinct prime factors.
 * https://oeis.org/A328855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of factorizations of n into numbers with an odd number of
 *            distinct prime factors
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>


static bool *odd_arr = NULL;          /* odd_arr[i] == true iff i has an odd number of distinct prime factors */
static idx_t max_precompute = 1;      /* largest n for which odd_arr is valid */

/* Ensure odd_arr is computed for all numbers up to limit. */
static void ensure_precompute(idx_t limit)
{
    if (limit <= max_precompute)
        return;

    /* Allocate array for smallest prime factor */
    idx_t *spf = (idx_t *)malloc((limit + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= limit; ++i)
        spf[i] = 0;
    if (limit >= 1)
        spf[1] = 1;

    /* Sieve to compute smallest prime factor for each number */
    for (idx_t i = 2; i <= limit; ++i)
    {
        if (spf[i] == 0)
        {
            spf[i] = i;
            if (i > limit / i)      /* avoid overflow of i*i */
                continue;
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    }

    /* Resize odd_arr */
    odd_arr = (bool *)realloc(odd_arr, (limit + 1) * sizeof(bool));

    /* Compute odd_arr using spf */
    odd_arr[0] = false;
    odd_arr[1] = false;
    for (idx_t i = 2; i <= limit; ++i)
    {
        idx_t x = i;
        int cnt = 0;
        while (x > 1)
        {
            idx_t p = spf[x];
            ++cnt;
            while (x % p == 0)
                x /= p;
        }
        odd_arr[i] = (cnt & 1) ? true : false;
    }

    free(spf);
    max_precompute = limit;
}

/* Recursive computation of the number of factorizations of n
   with all factors having an odd number of distinct prime factors.
   min_factor enforces non‑decreasing order to avoid duplicate counts. */
static seq_t dfs(idx_t n, idx_t min_factor)
{
    if (n == 1)
        return 1;

    seq_t total = 0;

    /* Check the factorization consisting of n itself */
    if (n >= min_factor && odd_arr[n])
        ++total;

    /* Enumerate divisors d of n that are >= min_factor and <= sqrt(n) */
    for (idx_t d = min_factor; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if (odd_arr[d])
            {
                total += dfs(n / d, d);
            }
        }
    }

    return total;
}

/* Cache for already computed a(n) values */
static seq_t *a_cache = NULL;
static idx_t max_cached = 0;

/* Public function as required by the build. */
seq_t a(idx_t n)
{
    /* Ensure limits for precomputation */
    if (n == 0)
        return 0;        /* Undefined for n = 0; return 0 to be safe */
    
    ensure_precompute(n);

    /* Return cached result if available */
    if (n <= max_cached && a_cache[n] != 0)
        return a_cache[n];

    /* Compute value */
    seq_t value = dfs(n, 2);

    /* Resize cache if necessary */
    if (n > max_cached)
    {
        a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        /* Initialize new entries to 0 */
        for (idx_t i = max_cached + 1; i <= n; ++i)
            a_cache[i] = 0;
        max_cached = n;
    }

    /* Store and return */
    a_cache[n] = value;
    return value;
}
