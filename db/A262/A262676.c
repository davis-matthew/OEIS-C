/*
 * A262676  Number of nonzero even numbers encountered when iterating A049820
 *          starting from n: a(0) = 0 and for n >= 1, a(n) = (1-A000035(n)) +
 *          a(A049820(n)).
 * https://oeis.org/A262676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; for n ≥ 1, a(n) = (n is even? 1 : 0) + a(n – d(n)),
 *            where d(n) = number of divisors of n. This counts the even
 *            numbers (including the starting n) encountered before reaching
 *            zero when repeatedly applying k ← k – d(k)
 * Range:     u8 n <= 2301, u16 n <= 9463, u32 n <= 9463, u64 n <= 9463,
 *            u128 n <= 9463, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static idx_t *spf   = NULL;          /* smallest prime factor table     */
static size_t spf_cap = 0;           /* current capacity of spf         */

static seq_t *cache = NULL;          /* memoized a(n) values             */
static size_t cache_cap = 0;         /* capacity of cache array          */
static idx_t  last_computed = 0;     /* largest n for which cache[n] is valid */

/* Ensure that the smallest prime factor table covers [0,limit]. */
static void ensure_spf(idx_t limit)
{
    if ((size_t)limit + 1 <= spf_cap) return;

    size_t new_cap = (size_t)limit + 1;
    idx_t *new_spf = (idx_t *)realloc(spf, new_cap * sizeof(idx_t));
    if (!new_spf) exit(EXIT_FAILURE);
    spf = new_spf;
    spf_cap = new_cap;

    for (idx_t i = 0; i <= limit; ++i) spf[i] = i;
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (spf[i] == i) {                     /* i is prime            */
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == j) spf[j] = (idx_t)i;
            }
        }
    }
}

/* Return the divisor count d(n) using the spf table (d(1)=1). */
static idx_t divisor_count(idx_t n)
{
    if (n <= 1) return 1;
    idx_t count = 1;
    while (n > 1) {
        idx_t p = spf[n];
        idx_t e = 0;
        while (n % p == 0) { n /= p; ++e; }
        count *= (e + 1);
    }
    return count;
}

/* Expand the memoization cache up to index n. */
static void ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap) return;

    /* Make sure the spf table is ready for all numbers up to n. */
    ensure_spf(n);

    size_t new_cap = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    cache_cap = new_cap;

    /* Guarantee cache[0] exists and is zero. */
    if (cache_cap > 0) cache[0] = (seq_t)0;

    /* Compute values from last_computed+1 up to n. */
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        idx_t d = divisor_count(k);
        seq_t even = (k & (idx_t)1) ? (seq_t)0 : (seq_t)1;
        cache[k] = even + cache[k - d];
    }
    last_computed = n;
}

/* Public function requested by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
