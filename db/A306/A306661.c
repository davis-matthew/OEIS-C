/*
 * A306661  Numbers with chained divisors: Numbers k with divisors such that
 *          the last digit of every divisor is the same as the first digit of
 *          the next divisor.
 * https://oeis.org/A306661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = numbers with chained divisors (brute‑force search with
 *            caching)
 * Range:     u8 n <= 27, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digits_first, digits_last, divisors_from_factorization
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digits_first.h"
#include "../utils/digits_last.h"
#include "../utils/divisors_from_factorization.h"

/* ---------- internal utilities ---------- */

static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/* integer square root (floor) using binary search and __int128 for safety */
static uint64_t isqrt64(uint64_t n)
{
    uint64_t low = 0, high = n, mid;
    while (low <= high) {
        mid = low + (high - low) / 2;
        __int128 prod = (__int128)mid * mid;
        if (prod == n) return mid;
        if (prod < n) low = mid + 1;
        else high = mid - 1;
    }
    return high;
}

/* ------------ prime list management ------------ */

static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static size_t prime_cap   = 0;

static void ensure_primes_up_to(uint64_t limit)
{
    if (prime_count == 0) {
        prime_cap = 16;
        prime_list = (uint64_t *)malloc(prime_cap * sizeof(uint64_t));
        prime_list[prime_count++] = 2;
    }
    uint64_t candidate = prime_list[prime_count - 1] + 1;
    while (candidate <= limit) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count &&
                prime_list[i] * prime_list[i] <= candidate; ++i) {
            if (candidate % prime_list[i] == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_count == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint64_t *)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_count++] = candidate;
        }
        ++candidate;
    }
}

/* factorization of n using pre‑computed primes */
static size_t factor_uint64(uint64_t n,
                            uint64_t *factors, size_t *exps,
                            size_t max_factors)
{
    size_t cnt = 0;
    uint64_t m = n;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= m; ++i) {
        if (cnt >= max_factors) break;
        uint64_t p = prime_list[i];
        if (m % p == 0) {
            size_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            factors[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (m > 1 && cnt < max_factors) {
        factors[cnt] = m;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* test the chained‑divisors property */
static bool is_chain_divisor(uint64_t n)
{
    if (n == 1) return true;

    uint64_t sqrt_n = isqrt64(n);
    ensure_primes_up_to(sqrt_n);

    /* using a fixed maximum number of distinct prime factors */
    uint64_t factors[16];
    uint64_t exps[16];
    size_t fac_cnt = factor_uint64(n, factors, exps, 16);

    size_t div_len;
    uint64_t *divs = divisors_from_factorization(factors, exps,
                                                 fac_cnt, &div_len);
    /* sort ascending */
    qsort(divs, div_len, sizeof(uint64_t), cmp_uint64);

    bool ok = true;
    for (size_t i = 0; i + 1 < div_len && ok; ++i) {
        uint64_t last = digits_last(divs[i]);
        uint64_t first = digits_first(divs[i + 1]);
        if (last != first) ok = false;
    }
    free(divs);
    return ok;
}

/* ------------ sequence storage (static) ------------ */

static seq_t *cache = NULL;       /* cache[0] corresponds to a(1) */
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_candidate = 2;    /* next integer to test */
static bool cache_inited = false;

/* grow cache to at least new_len */
static void grow_cache(size_t new_len)
{
    if (new_len <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < new_len) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, safe to return 0 */

    if (!cache_inited) {
        /* initialize cache with a(1) = 1 */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache_len = 1;
        next_candidate = 2;
        cache_inited = true;
    }

    /* fast return if already cached */
    if ((size_t)n <= cache_len) return cache[(size_t)n - 1];

    /* search forward until we have n terms */
    while ((size_t)n > cache_len) {
        uint64_t cand = next_candidate++;
        if (is_chain_divisor(cand)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[(size_t)n - 1];
}
