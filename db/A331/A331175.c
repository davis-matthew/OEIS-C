/*
 * A331175  Number of values of k, 1 <= k <= n, with A109395(k) = A109395(n),
 *          where A109395(n) = n/gcd(n, phi(n)).
 * https://oeis.org/A331175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k, 1 <= k <= n, with A109395(k) = A109395(n), *
 *            where A109395(k) = k / gcd(k,phi(k)). | * Computation: maintain
 *            cumulative counts of each value seen so far * while extending
 *            the cache up to the required n
 * Range:     u8 n <= 468, u16 n <= 9653, u32 n <= 9653, u64 n <= 9653,
 *            u128 n <= 9653, big any n
 * Helpers:   gcd, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

#include "../utils/gcd.h"
#include "../utils/phi.h"

/* Cache of a(n) values.  Index 0 is unused to keep a(1) at a_cache[1].  */
static seq_t *a_cache   = NULL;
/* Capacity of a_cache (last valid index).  */
static size_t a_cap     = 0;

/* Frequency of each A109395 value among all k <= current_max_n.
 * Value 'v' (which is <= current_max_n) has its count stored at
 * freq[v].  */
static size_t *freq     = NULL;
/* Capacity of freq (last valid index).  */
static size_t freq_cap  = 0;

/* Current maximum n for which a_cache and freq are fully populated.  */
static size_t current_max_n = 0;

/* Ensure caches large enough to hold indices up to n (inclusive).  */
static void ensure_capacity(size_t n)
{
    if (n <= a_cap) return;

    /* Resize a_cache.  */
    size_t new_cap = n;
    seq_t *tmp_a = (seq_t *)realloc(a_cache, (new_cap + 1) * sizeof(seq_t));
    if (!tmp_a) exit(1);
    a_cache = tmp_a;
    a_cap   = new_cap;

    /* Resize freq.  */
    size_t *tmp_f = (size_t *)realloc(freq, (new_cap + 1) * sizeof(size_t));
    if (!tmp_f) exit(1);
    freq = tmp_f;
    size_t old_cap = freq_cap;
    freq_cap = new_cap;

    /* Zero the newly allocated freq entries.  */
    for (size_t i = old_cap + 1; i <= new_cap; ++i) freq[i] = 0;
}

/* Increment frequency for a given value (which is guaranteed <= n) and
 * return the new count.  */
static size_t increment_freq(size_t val)
{
    ++freq[val];
    return freq[val];
}

/* Main function.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, return 0 as guard */
    size_t N = (size_t)n;          /* use native x86_64 size_t for indexing */

    /* Extend caches if this n is larger than all previously processed.  */
    if (N > current_max_n) {
        ensure_capacity(N);

        /* Compute A109395(k) for each new k and update frequency and a_cache.  */
        for (size_t k = current_max_n + 1; k <= N; ++k) {
            seq_t phi_k   = phi((idx_t)k);
            seq_t denom   = gcd((seq_t)k, phi_k);
            seq_t val_k   = (seq_t)k / denom;
            size_t v      = (size_t)val_k;   /* val_k <= k <= N <= 2^64 */
            size_t count  = increment_freq(v);
            a_cache[k] = (seq_t)count;
        }
        current_max_n = N;
    }

    return a_cache[N];
}
