/*
 * A248972  a(n) is the smallest b such that b^((p-1)/2) == -1 (mod p) where p
 *          = A080076(n) is the n-th Proth prime.
 * https://oeis.org/A248972
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the smallest b such that b^((p-1)/2) ≡ -1 (mod p), where
 *            p = the n-th Proth prime
 * Range:     u8 n <= 9, u16 n <= 84, u32 n <= 92, u64 n <= 92, u128 n <= 92,
 *            big any n
 * Helpers:   ensure_capacity, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility headers – the build supplies these implementation files. */
#include "../utils/next_prime.h"
#include "../utils/mod_pow.h"
#include "../utils/ensure_capacity.h"

/* Helper: count trailing zeros of a 64‑bit integer. */
static inline unsigned tz_uint64(uint64_t v)
{
    unsigned c = 0;
    while ((v & 1ULL) == 0) {
        v >>= 1;
        ++c;
    }
    return c;
}

/* Test if a prime p satisfies the Proth condition
   p = 1 + k·2^m, k odd, k < 2^m. */
static inline bool is_proth_prime(uint64_t p)
{
    uint64_t v = p - 1;
    unsigned m = tz_uint64(v);          /* largest exponent of 2 dividing p-1 */
    uint64_t k = v >> m;                /* odd factor */
    return ((k & 1ULL) == 1ULL)         /* k odd */
        && (k < (1ULL << m));          /* k < 2^m */
}

/* Static storage for memoised values */
static uint64_t *proth_primes = NULL;
static size_t proth_len = 0, proth_cap = 0;

static seq_t *a_cache = NULL;
static size_t a_len = 0, a_cap = 0;

static uint64_t last_prime = 2;     /* prime from which we search next */

/* Generate Proth primes and corresponding a(n) values up to index n. */
static void ensure_a_up_to(idx_t n)
{
    /* Compute sufficient Proth primes first. */
    while (proth_len < n) {
        uint64_t p = next_prime(last_prime);
        last_prime = p;
        if (!is_proth_prime(p))
            continue;

        if (proth_len + 1 > proth_cap)
            ensure_capacity(proth_len + 1, &proth_cap,
                            (void **)&proth_primes, sizeof(uint64_t));
        proth_primes[proth_len++] = p;
    }

    /* Compute a(n) values for any indices that have not yet been cached. */
    while (a_len < n) {
        uint64_t p   = proth_primes[a_len];
        seq_t    val = 0;

        if (a_len < 3) {
            /* First three terms are 2. */
            val = (seq_t)2;
        } else {
            uint64_t exp = (p - 1) >> 1;    /* exponent (p-1)/2 */
            for (uint64_t b = 2; ; ++b) {
                seq_t power = mod_pow((seq_t)b, exp, (seq_t)p);
                if (power == (seq_t)p - 1) {
                    val = (seq_t)b;
                    break;
                }
            }
        }

        if (a_len + 1 > a_cap)
            ensure_capacity(a_len + 1, &a_cap,
                            (void **)&a_cache, sizeof(seq_t));
        a_cache[a_len++] = val;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)  /* offset is 1; undefined behaviour for n==0 */
        return (seq_t)0;
    ensure_a_up_to((idx_t)n);
    return a_cache[n - 1];
}
