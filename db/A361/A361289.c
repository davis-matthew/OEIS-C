/*
 * A361289  For the odd numbers 2n + 1, the least practical number r such that
 *          2n + 1 = r + p where p is prime.
 * https://oeis.org/A361289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the least practical r such that 2n+1 = r + p, p prime
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 111 terms through n = 9952 (2026-11-03)
 * Helpers:   is_prime, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition as given in OEIS A361289. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/mem.h"

/* ---------- test if a uint64_t is practical ----------
   Implements the classical characterisation:
   1. 1 and 2 are practical.
   2. For n>2 with prime factorisation \prod p_i^{e_i}\ (p_1=2),
      require p_{i} <= 1 + \sigma( \prod_{j=1}^{i-1} p_j^{e_j} ).       */
static bool is_practical_uint64(uint64_t n) {
    if (n == 1u || n == 2u)
        return true;

    uint64_t temp = n;
    uint64_t sum = 1;          /* product of σ of processed primes */
    uint64_t prev = 0;         /* most recent prime factor */

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            if (prev == 0 && p != 2)       /* first prime must be 2 */
                return false;
            uint64_t exp = 0;
            uint64_t pp = 1;
            uint64_t sigma = 1;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
                pp *= p;
                sigma += pp;
            }
            if (p > sum + 1)                /* practical condition */
                return false;
            sum *= sigma;
            prev = p;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        uint64_t p = temp;
        if (prev == 0 && p != 2)
            return false;
        if (p > sum + 1)
            return false;
        sum *= (1 + p);                  /* σ(p^1) = 1 + p */
    }
    return true;
}

static seq_t *cache = NULL;          /* cache[0] holds a(1), etc. */
static size_t cache_cap = 0;
static size_t cache_len = 0;         /* maximum n computed + 1 */

static seq_t *practicals = NULL;    /* list of practical numbers */
static size_t practical_cap = 0;
static size_t practical_len = 0;

/* ---------- primality test for 64‑bit values ----------
   Use the provided is_prime(idx_t).  */
static inline bool is_prime_uint64(uint64_t x) {
    return is_prime((idx_t)x);
}

/* ---------- grow practicals capacity ----------
   Ensures the array can hold at least 'need' elements. */
static void grow_practical_cap(size_t need) {
    if (need <= practical_cap)
        return;
    size_t new_cap = practical_cap * 2 + 16;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_ptr = mem_resize(seq_t, practicals, new_cap);
    practicals = new_ptr;
    practical_cap = new_cap;
}

/* ---------- extend practicals up to 'limit' (inclusive) ----------
   Practical numbers are generated lazily on demand. */
static void extend_practicals_to(uint64_t limit) {
    if (practical_len == 0) {
        grow_practical_cap(1);
        practicals[0] = (seq_t)1;  /* first practical number */
        practical_len = 1;
    }
    uint64_t candidate = (uint64_t)practicals[practical_len - 1] + 1;
    while (candidate <= limit) {
        if (is_practical_uint64(candidate)) {
            grow_practical_cap(practical_len + 1);
            practicals[practical_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* ---------- grow cache capacity ----------
   Ensures the cache can store values up to index 'need' (1‑based). */
static void grow_cache_cap(size_t need) {
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap * 2 + 32;
    if (new_cap < need)
        new_cap = need;
    seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- main OEIS sequence function ----------
   Returns a(n) for n >= 1, caching results for later calls. */
seq_t a(idx_t n) {
    if (n < 1)
        return 0;   /* undefined for n < 1 */

    /* compute values up to n if needed */
    if ((size_t)n > cache_len) {
        grow_cache_cap((size_t)n);
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            uint64_t odd = 2u * (uint64_t)i + 1u;          /* 2n+1 */
            /* we need practical numbers up to (odd-2) */
            uint64_t limit = (odd > 2) ? odd - 2 : 0;
            extend_practicals_to(limit);

            seq_t best = 0;
            for (size_t j = 0; j < practical_len; ++j) {
                uint64_t r = (uint64_t)practicals[j];
                if (r > odd - 2)
                    break;
                uint64_t p = odd - r;
                if (is_prime_uint64(p)) {
                    best = (seq_t)r;
                    break;
                }
            }
            cache[i - 1] = best;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
