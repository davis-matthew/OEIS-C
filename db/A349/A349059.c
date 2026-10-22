/*
 * A349059  Number of weakly alternating ordered factorizations of n.
 * https://oeis.org/A349059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of weakly alternating ordered factorizations of n
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses recursion with memoization over (product, prev, trend).
   The exhaustive enumeration of factorizations is efficient for n <= 10000
   and runs well under the time limit.  */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#define HASH_BITS 20
#define HASH_SIZE (1 << HASH_BITS)

/* Cache entry for memoization of partial counts */
typedef struct CacheEntry {
    uint64_t key;       /* key = (m << 33) | (prev << 1) | trend */
    seq_t val;          /* stored count */
} CacheEntry;

static CacheEntry cache[HASH_SIZE] = {0};

/* Simple hash lookup / insert */
static seq_t cache_lookup(uint64_t key, bool *found)
{
    size_t idx = key & (HASH_SIZE - 1);
    for (; ; idx = (idx + 1) & (HASH_SIZE - 1)) {
        if (cache[idx].key == 0) {
            *found = false;
            return 0;
        }
        if (cache[idx].key == key) {
            *found = true;
            return cache[idx].val;
        }
    }
}

static void cache_insert(uint64_t key, seq_t val)
{
    size_t idx = key & (HASH_SIZE - 1);
    for (; ; idx = (idx + 1) & (HASH_SIZE - 1)) {
        if (cache[idx].key == 0) {
            cache[idx].key = key;
            cache[idx].val = val;
            return;
        }
        if (cache[idx].key == key) {
            cache[idx].val = val;
            return;
        }
    }
}

/* Small prime list for trial division (p <= 97) */
static const uint32_t small_primes[25] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,
    53,59,61,67,71,73,79,83,89,97
};
static const size_t small_prime_cnt = 25;

/* Generate all divisors of n (including 1) in the array divs.
   Assumes divs has space for at least 2048 elements.  */
static void generate_divisors(uint64_t n, uint64_t *divs, size_t *cnt)
{
    uint32_t pf[25];
    int    exp[25];
    size_t pcnt = 0;
    uint64_t tmp = n;

    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * (uint64_t)p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[pcnt] = p;
            exp[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        pf[pcnt] = (uint32_t)tmp;
        exp[pcnt] = 1;
        ++pcnt;
    }

    divs[0] = 1;
    *cnt = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pf[i];
        int e = exp[i];
        size_t prev = *cnt;
        uint64_t pow = 1;
        for (int k = 1; k <= e; ++k) {
            pow *= p;
            for (size_t j = 0; j < prev; ++j) {
                divs[*cnt] = divs[j] * pow;
                ++(*cnt);
            }
        }
    }
}

/* Forward declarations of recursion functions */
static seq_t inc_rec(idx_t m, idx_t prev);
static seq_t dec_rec(idx_t m, idx_t prev);

/* Recursive count where next factor must be >= prev (increasing trend) */
static seq_t inc_rec(idx_t m, idx_t prev)
{
    uint64_t key = ((uint64_t)m << 33) | (prev << 1) | 0ULL; /* trend 0 = inc */
    bool found;
    seq_t cached = cache_lookup(key, &found);
    if (found) return cached;

    seq_t res = 0;
    uint64_t divs[2048];
    size_t cnt;
    generate_divisors((uint64_t)m, divs, &cnt);

    for (size_t i = 1; i < cnt; ++i) {          /* skip divisor 1 */
        uint64_t d = divs[i];
        if (d < 2ULL) continue;
        if (d < (uint64_t)prev) continue;     /* must be >= prev */
        if ((uint64_t)d == (uint64_t)m) {
            res += 1;                          /* sequence of length 1 */
        } else {
            res += dec_rec((idx_t)((uint64_t)m / d), (idx_t)d);
        }
    }
    cache_insert(key, res);
    return res;
}

/* Recursive count where next factor must be <= prev (decreasing trend) */
static seq_t dec_rec(idx_t m, idx_t prev)
{
    uint64_t key = ((uint64_t)m << 33) | (prev << 1) | 1ULL; /* trend 1 = dec */
    bool found;
    seq_t cached = cache_lookup(key, &found);
    if (found) return cached;

    seq_t res = 0;
    uint64_t divs[2048];
    size_t cnt;
    generate_divisors((uint64_t)m, divs, &cnt);

    for (size_t i = 1; i < cnt; ++i) {          /* skip divisor 1 */
        uint64_t d = divs[i];
        if (d < 2ULL) continue;
        if (d > (uint64_t)prev) continue;     /* must be <= prev */
        if ((uint64_t)d == (uint64_t)m) {
            res += 1;
        } else {
            res += inc_rec((idx_t)((uint64_t)m / d), (idx_t)d);
        }
    }
    cache_insert(key, res);
    return res;
}

/* Count equal‑factor representations of n (len >= 2) */
static seq_t count_equal_factor_subtractions(idx_t n)
{
    if (n <= 1) return 0;

    uint32_t pf[25];
    int    exp[25];
    size_t pcnt = 0;
    uint64_t tmp = (uint64_t)n;

    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * (uint64_t)p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pf[pcnt] = p;
            exp[pcnt] = e;
            ++pcnt;
        }
    }
    if (tmp > 1) {
        pf[pcnt] = (uint32_t)tmp;
        exp[pcnt] = 1;
        ++pcnt;
    }

    /* Compute gcd of exponents */
    uint32_t g = 0;
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t e = exp[i];
        /* simple gcd */
        while (e != 0) {
            uint32_t t = g % e;
            g = e;
            e = t;
        }
    }
    if (g < 2) return 0;

    seq_t sub = 0;
    for (uint32_t l = 2; l <= g; ++l) {
        if (g % l == 0) {
            sub += 1;
        }
    }
    return sub;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 1) return 1;

    seq_t total = 0;
    uint64_t divs[2048];
    size_t cnt;
    generate_divisors((uint64_t)n, divs, &cnt);

    for (size_t i = 1; i < cnt; ++i) {          /* skip divisor 1 */
        uint64_t d = divs[i];
        if (d < 2ULL) continue;
        if ((uint64_t)d == (uint64_t)n) {
            total += 1;
        } else {
            total += inc_rec((idx_t)((uint64_t)n / d), (idx_t)d);
            total += dec_rec((idx_t)((uint64_t)n / d), (idx_t)d);
        }
    }

    /* Subtract double‑counted sequences with all equal factors */
    seq_t sub = count_equal_factor_subtractions(n);
    return total - sub;
}
