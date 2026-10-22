/*
 * A346014  Numbers whose average number of distinct prime factors of their
 *          divisors is an integer.
 * https://oeis.org/A346014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence A346014:
   Numbers k such that the average of the number of distinct prime
   factors of all divisors of k is an integer.
   A term k satisfies: Σ e_i/(e_i+1) is an integer, where e_i are the
   exponents in k's prime factorisation.
   The function dynamically builds the sequence in increasing order
   and memoises results. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

static uint32_t *prime_list = NULL;
static size_t prime_count = 0;

/* Initialise a list of primes up to 1 000 000 using a simple sieve. */
static void init_primes(void) {
    if (prime_list) return;
    const uint32_t LIMIT = 1000000;
    bool *is_comp = (bool*)calloc(LIMIT + 1, sizeof(bool));
    size_t capacity = LIMIT / 10;
    prime_list = (uint32_t*)malloc(capacity * sizeof(uint32_t));
    size_t cnt = 0;
    for (uint32_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            if (cnt == capacity) {
                capacity *= 2;
                prime_list = (uint32_t*)realloc(prime_list, capacity * sizeof(uint32_t));
            }
            prime_list[cnt++] = i;
            if ((uint64_t)i * i <= LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= LIMIT; j += i)
                    is_comp[(size_t)j] = true;
            }
        }
    }
    prime_count = cnt;
    free(is_comp);
}

/* Euclidean GCD for seq_t. Works for built‑in integers and for
   arbitrary‑precision big integers because operators % and == are
   supported. */
static seq_t seq_gcd(seq_t a, seq_t b) {
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Return true if z satisfies the average‑integer condition. */
static bool satisfies(uint64_t z) {
    if (z == 1) return true;          /* 1 has average 0 */
    init_primes();

    /* Factorisation – store only exponents. */
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t n = z;
    for (size_t i = 0; i < prime_count && (uint64_t)prime_list[i] * prime_list[i] <= n; ++i) {
        uint32_t p = prime_list[i];
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }
    if (n > 1) {               /* remaining prime factor */
        exps[cnt++] = 1;
    }

    /* Compute lcm of (e_i+1) and Σ e_i * lcm/(e_i+1). */
    seq_t lcm = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t denom = (seq_t)(exps[i] + 1);
        seq_t g = seq_gcd(lcm, denom);
        lcm = (lcm / g) * denom;
    }

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t denom = (seq_t)(exps[i] + 1);
        seq_t mult  = lcm / denom;
        seq_t term  = (seq_t)exps[i] * mult;
        sum += term;
    }
    return (sum % lcm) == (seq_t)0;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static uint64_t next_candidate = 1;

    if (!cache) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
    }

    while (cache_len < n) {
        if (satisfies(next_candidate)) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
