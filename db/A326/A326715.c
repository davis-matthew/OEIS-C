/*
 * A326715  Values of n for which the denominator of (Sum_{prime p | n} 1/p -
 *          1/n) is 1.
 * https://oeis.org/A326715
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th smallest integer in {1} ∪ primes ∪ Giuga numbers
 *            (Giuga numbers satisfy ∀ prime p|n, n % (p*p) == p). This
 *            function builds the set incrementally by testing each integer
 *            for primality or the Giuga condition
 * Range:     u8 n <= 56, u16 n <= 6546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static idx_t cur_limit = 1;                 /* last integer considered */

static idx_t *prime_list = NULL;
static size_t prime_cnt = 0, prime_cap = 0;

/* Append a value to the cache, resizing if needed */
static void push_cache(seq_t v) {
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap << 1;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    }
    cache[cache_len++] = v;
}

/* Append a prime to the prime list, resizing if needed */
static void push_prime(idx_t p) {
    if (prime_cnt == prime_cap) {
        prime_cap = (prime_cap == 0) ? 16 : prime_cap << 1;
        prime_list = (idx_t *)realloc(prime_list, prime_cap * sizeof(idx_t));
    }
    prime_list[prime_cnt++] = p;
}

/* Simple deterministic primality test using the known primes */
static bool isPrime(idx_t n) {
    if (n < 2) return false;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > n / p) break;          /* p*p > n */
        if (n % p == 0) return false;
    }
    push_prime(n);
    return true;
}

/* Giuga test for composite n using prime factors from prime_list */
static bool isGiuga(idx_t n) {
    idx_t tmp = n;
    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = prime_list[i];
        if (p > tmp / p) break;
        if (tmp % p == 0) {            /* p divides n */
            if (n % (p * p) != p) return false;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) {                     /* remaining factor is prime */
        if (n % (tmp * tmp) != tmp) return false;
    }
    return true;
}

/* Generate all numbers from cur_limit+1 up to limit inclusive */
static void generate_up_to(idx_t limit) {
    for (idx_t i = cur_limit + 1; i <= limit; ++i) {
        bool ok = false;
        if (i == 1) {
            ok = true;
        } else if (isPrime(i)) {
            ok = true;
        } else if (isGiuga(i)) {
            ok = true;
        }
        if (ok) push_cache((seq_t)i);
    }
    cur_limit = limit;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                           /* safety */
    if (cache_len == 0) {                           /* first call */
        push_cache((seq_t)1);                       /* 1 is in the set */
    }
    while (cache_len < n) {
        /* Extend the search range; step size can be tuned */
        idx_t step = 10000;
        idx_t new_limit = cur_limit + step;
        if (new_limit < cur_limit) new_limit = ~((idx_t)0); /* overflow guard */
        generate_up_to(new_limit);
    }
    return cache[n - 1];
}
