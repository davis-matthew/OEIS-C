/*
 * A343973  Primes p such that both p-1 and p+1 are in A093641.
 * https://oeis.org/A343973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-03)
 * Helpers:   is_prime_u64, mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A343973: primes p such that both p-1 and p+1 contain at most one odd prime factor.
   The routine generates primes of the form 3*2^k±1, checks the condition,
   and keeps the results in a memoised cache so that calls with any order
   are handled efficiently. */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Helper: return true if x has at most one odd prime factor (counted with multiplicity) */
static bool has_at_most_one_odd_prime_factor(seq_t x)
{
    /* Remove factors of 2 */
    while ((x & 1) == 0) {
        x /= 2;
    }
    /* x is now odd. It must be 1 or a prime to satisfy the condition. */
    if (x == 1) {
        return true;
    }
    return is_prime_big(x);
}

/* Helper: true if p is a prime satisfying the A343973 condition (tested on p-1 and p+1) */
static bool candidate_ok(seq_t p)
{
    if (!is_prime_big(p)) {
        return false;
    }
    if (!has_at_most_one_odd_prime_factor(p - 1)) {
        return false;
    }
    if (!has_at_most_one_odd_prime_factor(p + 1)) {
        return false;
    }
    return true;
}

/* Add a term to the cache, expanding capacity if needed */
static void add_term(seq_t **cache, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        *cap *= 2;
        *cache = mem_resize(seq_t, *cache, (*cap * sizeof(seq_t)) / sizeof(seq_t));
    }
    (*cache)[*len] = val;
    (*len)++;
}

seq_t a(idx_t n)
{
    /* Static state: memoised sequence and next exponent to test */
    static seq_t *cache = NULL;   /* array of generated terms */
    static size_t cache_len = 0;  /* number of entries currently stored */
    static size_t cache_cap = 0;  /* allocated capacity */
    static idx_t next_k = 2;      /* start test at k = 2 (k=1 already covered) */

    /* Initialise cache lazily, inserting the first four known terms */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)7;
        cache_len = 4;
    }

    /* Generate new terms until we have at least n entries */
    while (cache_len < n) {
        seq_t pow2_k = pow2((idx_t)next_k);
        seq_t base  = mul_small(pow2_k, 3);          /* 3*2^k */

        /* Candidate p = 3*2^k - 1 */
        seq_t p_minus  = base - 1;
        if (candidate_ok(p_minus)) {
            add_term(&cache, &cache_len, &cache_cap, p_minus);
        }

        /* Candidate p = 3*2^k + 1 */
        seq_t p_plus   = base + 1;
        if (candidate_ok(p_plus)) {
            add_term(&cache, &cache_len, &cache_cap, p_plus);
        }

        next_k++;   /* move to next exponent */
    }

    return cache[n - 1];  /* 1‑based indexing */
}
