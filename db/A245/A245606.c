/*
 * A245606  Permutation of natural numbers: a(1) = 1, a(2n) = 1 +
 *          A003961(a(n)), a(2n+1) = A003961(1+a(n)). [Where A003961(n) shifts
 *          the prime factorization of n one step left].
 * https://oeis.org/A245606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2n) = 1 + A003961(a(n)), a(2n+1) = A003961(1 +
 *            a(n)). Here A003961 shifts the prime factorization of its
 *            argument to the next primes. This implementation memoizes a()
 *            and factors its arguments using trial division with a
 *            pre‑computed prime table
 * Range:     u8 n <= 45, u16 n <= 471, u32 n <= 10001, u64 n <= 10001,
 *            u128 n <= 10001, big any n
 * Validated: standard, pass, 90 terms through n = 9432 (2026-11-03)
 * Helpers:   build_primes_upto, ipow_i32, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

/* Prime table used for factorisation: at most 1 000 000. */
static uint64_t *primes = NULL;
static size_t primes_cnt  = 0;
static bool primes_ready  = false;

static void init_primes(void)
{
    if (!primes_ready) {
        build_primes_upto(1000000, &primes, &primes_cnt);
        primes_ready = true;
    }
}

/* A003961: shift prime factorisation one step to the right.
   For 1 it returns 1 (empty factorisation). */
static seq_t a003961(seq_t n)
{
    if (n <= 1) return n;

    seq_t temp = n;
    seq_t result = 1;
    init_primes();

    seq_t sqrt_temp = floor_sqrt(temp);

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        seq_t sp = (seq_t)p;
        if (sp > sqrt_temp) break;
        if (temp % sp == 0) {
            int exp = 0;
            while (temp % sp == 0) {
                temp /= sp;
                ++exp;
            }
            uint64_t nxt = next_prime(p);
            result *= ipow_i32((seq_t)nxt, exp);
        }
    }

    if (temp > 1) { /* remaining prime factor */
        uint64_t r   = (uint64_t)temp;
        uint64_t nxt = next_prime(r);
        result *= (seq_t)nxt;
    }

    return result;
}

/* Memoised sequence a(n).  cache[1] is initialised to 1. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;     /* not used, but safety */

    ensure_cache(n, &cache, &cache_cap);

    if (cache[1] == 0) cache[1] = 1;   /* initialise base case */

    if (cache[n] != 0) return cache[n];

    if ((n & 1) == 0) {          /* even */
        idx_t k = n >> 1;
        seq_t v = a(k);
        cache[n] = 1 + a003961(v);
    } else {                     /* odd */
        idx_t k = (n - 1) >> 1;
        seq_t v = a(k);
        cache[n] = a003961(1 + v);
    }

    return cache[n];
}
