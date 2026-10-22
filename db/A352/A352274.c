/*
 * A352274  Numbers whose squarefree part is congruent to 1 modulo 6 or 3
 *          modulo 18.
 * https://oeis.org/A352274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A352274: numbers whose squarefree part is ≡ 1 (mod 6) or ≡ 3 (mod 18). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Cached sequence values */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t next_candidate = 1;

/* Prime list for trial division */
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static size_t prime_cap = 0;
static uint64_t last_checked = 2;

/* Ensure prime_list contains all primes <= limit */
static void ensure_primes_up_to(uint64_t limit)
{
    while (last_checked <= limit) {
        uint64_t cand = last_checked;
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > cand) break;
            if (cand % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                size_t new_cap = prime_cap ? prime_cap << 1 : 16;
                prime_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
                prime_cap = new_cap;
            }
            prime_list[prime_cnt++] = cand;
        }
        ++last_checked;
    }
}

/* Test whether n has squarefree part ≡ 1 (mod 6) or 3 (mod 18) */
static bool good_candidate(uint64_t n)
{
    uint64_t tmp = n;
    int mod18 = 1;

    /* Prime list needed for factorisation */
    ensure_primes_up_to(n);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > tmp) break;
        if (tmp % p == 0) {
            int e = 0;
            do {
                tmp /= p;
                ++e;
            } while (tmp % p == 0);
            if (e & 1) {                     /* odd exponent */
                mod18 = (int)((mod18 * (p % 18)) % 18);
            }
        }
    }

    if (tmp > 1) {                            /* remaining prime factor */
        mod18 = (int)((mod18 * (tmp % 18)) % 18);
    }

    /* Accept if squarefree part ≡ 1 (mod 6) or 3 (mod 18) */
    return ((mod18 % 6) == 1) || (mod18 == 3);
}

/* Ensure cache holds at least 'needed' terms */
static void extend_cache(idx_t needed)
{
    while (cache_len < needed) {
        /* Find next qualifying integer */
        while (true) {
            if (good_candidate((uint64_t)next_candidate)) {
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap << 1 : 128;
                    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache_cap = new_cap;
                }
                cache[cache_len++] = (seq_t)next_candidate;
                ++next_candidate;
                break;
            }
            ++next_candidate;
        }
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* defensive */
    extend_cache(n);
    return cache[n - 1];
}
