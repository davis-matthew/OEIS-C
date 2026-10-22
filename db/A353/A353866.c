/*
 * A353866  Heinz numbers of rucksack partitions. Every prime-power divisor
 *          has a different sum of prime indices.
 * https://oeis.org/A353866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353866: Heinz numbers of rucksack partitions.
   The function enumerates positive integers and tests each for the
   unique‑subset‑sum property of their prime‑index multiset. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* --------- prime sieve --------- */
#define PRIME_SIEVE_LIMIT 2000000
static bool comp[PRIME_SIEVE_LIMIT + 1];
static uint32_t primes[PRIME_SIEVE_LIMIT];
static size_t prime_cnt = 0;
static bool sieve_done = false;

static void init_sieve(void)
{
    if (sieve_done) return;
    memset(comp, 0, sizeof(comp));
    for (uint32_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (!comp[i]) {
            primes[prime_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_SIEVE_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                    comp[j] = 1;
            }
        }
    }
    sieve_done = true;
}

/* binary search for prime index (1‑based) */
static uint32_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] == p) return (uint32_t)(mid + 1);
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0; /* should not happen for valid input */
}

/* gcd for 32‑bit values */
static uint32_t gcd_uint32(uint32_t a, uint32_t b)
{
    while (b) {
        uint32_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* check whether n is a Heinz number of a rucksack partition */
static bool is_rucksack_num(uint64_t n)
{
    if (n <= 1) return true;

    uint64_t tmp = n;
    uint32_t idx_arr[16];
    uint32_t exp_arr[16];
    size_t cnt = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            uint32_t e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            idx_arr[cnt] = (uint32_t)(i + 1);   /* 1‑based index of p */
            exp_arr[cnt] = e;
            ++cnt;
        }
    }
    if (tmp > 1) { /* remaining prime > sqrt(original n) */
        uint32_t idx = prime_index(tmp);
        if (idx == 0) return false;
        idx_arr[cnt] = idx;
        exp_arr[cnt] = 1;
        ++cnt;
    }

    /* pairwise conflict detection */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            uint32_t a = idx_arr[i];
            uint32_t b = idx_arr[j];
            uint32_t g = gcd_uint32(a, b);
            uint32_t need_a = b / g;   /* required exponent on i */
            uint32_t need_b = a / g;   /* required exponent on j */
            if (exp_arr[i] >= need_a && exp_arr[j] >= need_b)
                return false;
        }
    }
    return true;
}

/* --------- cache --------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap = 0;
static uint64_t last_test = 1;

static void ensure_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    if (cache_cap == 0) cache_cap = 64;
    while (cache_cap < new_cap) cache_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    cache = new_ptr;
}

/* --------- sequence function --------- */
seq_t a(idx_t n)
{
    init_sieve();
    if (n == 0) return (seq_t)0; /* undefined offset */

    while (cache_size < (size_t)n) {
        if (is_rucksack_num(last_test)) {
            ensure_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)last_test;
        }
        ++last_test;
    }
    return cache[n - 1];
}
