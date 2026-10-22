/*
 * A308213  Number of integer-sided triangles with perimeter n and sides a, b
 *          and c such that a <= b <= c and the length of side b is coprime to
 *          c.
 * https://oeis.org/A308213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Number of integer-sided triangles with perimeter n, sides a ≤ b
 *            ≤ c, and b coprime to c
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- Smallest‑prime‑factor table ----- */
static idx_t *spf = NULL;
static idx_t spf_limit = 0;

/* Build the SPF table up to 'limit'. */
static void ensure_spf(idx_t limit)
{
    if (limit <= spf_limit) return;
    spf = (idx_t *)realloc(spf, (limit + 1) * sizeof(idx_t));
    if (!spf) abort();
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i * i <= limit) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf_limit = limit;
}

/* Count integers 1..k that are coprime to n (n ≥ 1). */
static seq_t coprime_upto(idx_t n, idx_t k)
{
    if (k == 0) return 0;
    idx_t primes[32];
    size_t pcnt = 0;
    idx_t temp = n;
    while (temp > 1) {
        idx_t p = spf[temp];
        primes[pcnt++] = p;
        while (temp % p == 0) temp /= p;
    }
    seq_t cnt = k;
    for (size_t mask = 1; mask < (1ULL << pcnt); ++mask) {
        idx_t prod = 1;
        size_t bits = 0;
        for (size_t i = 0; i < pcnt; ++i) {
            if (mask & (1ULL << i)) {
                prod *= primes[i];
                ++bits;
                if (prod > k) break;
            }
        }
        if (prod > k) continue;
        seq_t val = k / prod;
        if (bits & 1) cnt -= val;
        else           cnt += val;
    }
    return cnt;
}

/* Compute a single term a(n). */
static seq_t compute_value(idx_t n)
{
    seq_t total = 0;
    if (n < 3) return 0;
    idx_t max_c = (n - 1) / 2;          /* largest side < n/2 */
    ensure_spf(max_c);
    for (idx_t c = 1; c <= max_c; ++c) {
        idx_t L = (n - c + 1) / 2;       /* ceil((n-c)/2) */
        if (L > c) continue;            /* no valid b for this c */
        seq_t up  = coprime_upto(c, c);
        seq_t low = coprime_upto(c, L - 1);
        total += up - low;
    }
    return total;
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;             /* highest index computed so far */

    if (n == 0) return 0;
    if (n > cache_size) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) abort();
        cache = new_cache;
        for (idx_t i = cache_size + 1; i <= n; ++i)
            cache[i] = compute_value(i);
        cache_size = n;
    }
    return cache[n];
}
