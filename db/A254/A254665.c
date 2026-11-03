/*
 * A254665  Primes prime(n) such that prime(n) + 7*n is also prime.
 * https://oeis.org/A254665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A254665: primes p such that p + 7*n is prime where n is the
   index of p among the primes (1‑based).  Example terms:
   a(1)=3, a(2)=71, a(3)=79, …  */
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "../utils/next_prime.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

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

/* Cache of already found terms.  cache[i] holds a(i+1). */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static size_t cache_cnt = 0;

/* Current state of prime generation.  prime_index counts primes seen,
   and last_prime holds the last generated prime.  The next prime is
   obtained by next_prime(last_prime). */
static idx_t  prime_index = 0;     /* 1‑based index of the last prime */
static uint64_t last_prime = 1;    /* start < 2 so next_prime(1)=2 */

/* Ensure that cache has room for at least one more element. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? (cache_cap << 1) : 64;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache     = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term of the sequence (n is 1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1 */

    /* Return cached value if available. */
    if (n <= cache_cnt) return cache[n - 1];

    /* Initialise cache on first use. */
    if (!cache) {
        cache_cap = 64;
        cache     = mem_new(seq_t, cache_cap);
        if (!cache) abort();
        cache_cnt = 0;
    }

    /* Generate primes until we have at least n terms in the sequence. */
    while (cache_cnt < (size_t)n) {
        /* Produce the next prime number. */
        last_prime = next_prime(last_prime);
        ++prime_index;

        /* Compute p + 7 * n where n is the prime's index. */
        seq_t sum = (seq_t)last_prime + ((seq_t)7 * (seq_t)prime_index);

        /* If the sum is also prime, record the prime. */
        if (is_prime_big(sum)) {
            if (cache_cnt >= cache_cap) grow_cache();
            cache[cache_cnt++] = (seq_t)last_prime;
        }
    }

    return cache[n - 1];
}
