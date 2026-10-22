/*
 * A317305  Sum of divisors of the n-th number whose divisors increase by a
 *          factor of 2 or less.
 * https://oeis.org/A317305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 2311, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula: a(n) = A000203(A174973(n))
   a(n) = sum of divisors of the n‑th integer whose sorted divisors
   increase by at most a factor of 2.
   Implementation compiles in C11 and C++17. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ------------------------------------ Utilities ------------------------------------ */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t va = *(const uint64_t *)a;
    uint64_t vb = *(const uint64_t *)b;
    return (va < vb) ? -1 : (va > vb) ? 1 : 0;
}

/* ------------------------------------ Prime sieve ---------------------------------- */
#define PRIME_LIMIT 200000
static uint32_t primes[PRIME_LIMIT];
static size_t prime_count = 0;
static bool primes_initialised = false;

static void init_primes(void)
{
    if (primes_initialised) return;
    bool *is_composite = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_count++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;
            }
        }
    }
    free(is_composite);
    primes_initialised = true;
}

/* ------------------------------------ Special number test & sigma ------------------- */
static bool special_and_sigma(uint64_t k, seq_t *out_sigma)
{
    if (k == 1) { *out_sigma = (seq_t)1; return true; }

    init_primes();

    uint64_t tmp = k;
    uint32_t fac[64];
    int exp[64];
    size_t nf = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac[nf] = p;
            exp[nf] = e;
            ++nf;
        }
    }
    if (tmp > 1) {
        fac[nf] = (uint32_t)tmp;
        exp[nf] = 1;
        ++nf;
    }

    /* Generate all divisors */
    uint64_t divs[1024];
    size_t dcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < nf; ++i) {
        uint32_t p = fac[i];
        int e = exp[i];
        uint64_t pow_e[20];
        pow_e[0] = 1;
        for (int j = 1; j <= e; ++j)
            pow_e[j] = pow_e[j - 1] * (uint64_t)p;

        size_t old = dcnt;
        for (size_t idx = 0; idx < old; ++idx) {
            uint64_t base = divs[idx];
            for (int j = 1; j <= e; ++j)
                divs[dcnt++] = base * pow_e[j];
        }
    }
    qsort(divs, dcnt, sizeof(uint64_t), cmp_uint64);

    /* Check consecutive ratio condition */
    for (size_t i = 0; i + 1 < dcnt; ++i) {
        if (divs[i + 1] > divs[i] * 2ULL) return false;
    }

    /* Compute sigma(k) */
    seq_t sigma = (seq_t)1;
    for (size_t i = 0; i < nf; ++i) {
        uint64_t p = fac[i];
        int e = exp[i];
        seq_t term = (seq_t)1;
        seq_t pp = (seq_t)1;
        for (int j = 0; j < e; ++j) {
            pp *= (seq_t)p;
            term += pp;
        }
        sigma *= term;
    }
    *out_sigma = sigma;
    return true;
}

/* ------------------------------------ Cached sequence -------------------------------- */
static idx_t last_checked_k = 1;          /* next k to test */
static seq_t *sigma_cache = NULL;         /* cached sigma values a(n) */
static size_t cache_cap = 0;              /* allocated cache size */
static size_t cache_len = 0;              /* number of terms stored */

static void cache_grow(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(sigma_cache, new_cap * sizeof(seq_t));
    sigma_cache = new_arr;
    cache_cap = new_cap;
}

/* ------------------------------------ Main function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for 0, but guard */

    /* Ensure the cache contains at least n terms */
    while (cache_len < (size_t)n) {
        seq_t sigma_val;
        if (special_and_sigma((uint64_t)last_checked_k, &sigma_val)) {
            cache_grow(cache_len + 1);
            sigma_cache[cache_len++] = sigma_val;
        }
        ++last_checked_k;
    }
    return sigma_cache[(size_t)n - 1];
}
