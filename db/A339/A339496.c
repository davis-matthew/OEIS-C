/*
 * A339496  T(n, k) = Sum(divisors(k) union {k*j : j = 2..floor(n/k)}).
 *          Triangle read by rows.
 * https://oeis.org/A339496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(r,k) = σ(k) + k * ( floor(r/k)*(floor(r/k)+1)/2 - 1 ) a(n) is
 *            obtained by flattening the triangle T(n,k) row‑by‑row with r =
 *            smallest integer such that r(r+1)/2 ≥ n and k = n - r(r-1)/2.
 *            (σ(k) = sum of divisors of k.) Implementation uses memoisation
 *            for both triangle entries and divisor sums, f...
 * Range:     u8 n <= 44, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;          /* cache for a(n)  (1‑based) */
static size_t cache_cap = 0;         /* current capacity of cache */

static seq_t *sigma_cache = NULL;    /* cache for σ(k)  (1‑based) */
static size_t sigma_cap = 0;         /* capacity for sigma_cache */

/* Compute σ(k) for small k with caching */
static seq_t sigma_of(idx_t k) {
    if (k == 0) return 0;
    if (k >= sigma_cap) {
        size_t new_cap = k + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure → return 0 */
        if (sigma_cache) {
            memcpy(new_cache + 1, sigma_cache + 1, (sigma_cap - 1) * sizeof(seq_t));
            free(sigma_cache);
        }
        memset(new_cache + 1, 0, (new_cap - 1) * sizeof(seq_t));
        sigma_cache = new_cache;
        sigma_cap = new_cap;
    }
    if (sigma_cache[k] != 0) return sigma_cache[k];

    idx_t remaining = k;
    seq_t result = 1;

    /* trial division */
    for (idx_t p = 2; (idx_t)p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            int exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }
            /* term = (p^(exp+1)-1)/(p-1) */
            seq_t p_pow = 1;   /* p^exp */
            for (int i = 0; i < exp; ++i) p_pow *= (seq_t)p;
            seq_t numerator = p_pow * (seq_t)p - 1;   /* p^(exp+1)-1 */
            seq_t denom = (seq_t)p - 1;
            seq_t term = numerator / denom;
            result *= term;
        }
    }
    if (remaining > 1) {            /* remaining is prime */
        seq_t prime = (seq_t)remaining;
        result *= (prime + 1);
    }

    sigma_cache[k] = result;
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;  /* undefined for n=0, return 0 */

    if (n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0; /* allocation failure */
        if (cache) {
            memcpy(new_cache + 1, cache + 1, (cache_cap - 1) * sizeof(seq_t));
            free(cache);
        }
        memset(new_cache + 1, 0, (new_cap - 1) * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }
    if (cache[n] != 0) return cache[n];

    /* find row r such that r(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tri = ((seq_t)mid * (mid + 1)) / 2;
        if (tri >= n) high = mid;
        else low = mid + 1;
    }
    idx_t r = low;
    seq_t prev_sum = ((seq_t)r * (r - 1)) / 2;
    idx_t k = (idx_t)(n - prev_sum);

    seq_t sigma = sigma_of(k);
    idx_t m = r / k;               /* floor(r/k) */
    seq_t m_seq = (seq_t)m;
    seq_t sum_j = m_seq * (m_seq + 1) / 2 - 1;   /* sum_{j=2}^{m} j */
    seq_t term = sigma + (seq_t)k * sum_j;

    cache[n] = term;
    return term;
}
