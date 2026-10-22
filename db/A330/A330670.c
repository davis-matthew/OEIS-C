/*
 * A330670  Squares of primes congruent to 1 (mod 30).
 * https://oeis.org/A330670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates squares of primes ending in 1 or 9 (OEIS A330670). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

static uint64_t *g_all_primes = NULL;     /* All primes found so far */
static size_t   g_prime_count = 0;
static size_t   g_prime_cap   = 0;

static seq_t *g_cache = NULL;             /* Cached sequence values */
static idx_t  g_cache_size = 0;           /* Number of values currently cached */
static size_t g_cache_cap   = 0;

static uint64_t g_cur_candidate = 11;      /* Next odd number to test */

/* Ensure we have primes enough to fill the cache up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= g_cache_size) return;

    /* Grow primes array if needed */
    if (g_prime_cap == 0) {
        g_prime_cap = 128;
        g_all_primes = (uint64_t*)malloc(g_prime_cap * sizeof(uint64_t));
    }
    /* Initialize base primes */
    if (g_prime_count == 0) {
        /* 2,3,5,7 are the first primes; we need them for trial division. */
        if (g_prime_cap < 4) {
            g_prime_cap = 4;
            g_all_primes = (uint64_t*)realloc(g_all_primes, g_prime_cap * sizeof(uint64_t));
        }
        g_all_primes[0] = 2;
        g_all_primes[1] = 3;
        g_all_primes[2] = 5;
        g_all_primes[3] = 7;
        g_prime_count = 4;
    }

    /* Grow cache array if needed */
    if (g_cache_cap == 0) {
        g_cache_cap = 128;
        g_cache = (seq_t*)malloc(g_cache_cap * sizeof(seq_t));
    }

    while (g_cache_size < n) {
        uint64_t c = g_cur_candidate;
        g_cur_candidate += 2;                 /* Only test odd numbers */

        /* Trial division using known primes */
        bool is_prime = true;
        for (size_t i = 0; i < g_prime_count; ++i) {
            uint64_t p = g_all_primes[i];
            if (p * p > c) break;
            if (c % p == 0) {
                is_prime = false;
                break;
            }
        }

        /* If c is prime, store it in the all_primes list */
        if (is_prime) {
            if (g_prime_count == g_prime_cap) {
                g_prime_cap *= 2;
                g_all_primes = (uint64_t*)realloc(g_all_primes, g_prime_cap * sizeof(uint64_t));
            }
            g_all_primes[g_prime_count++] = c;

            /* If it ends in 1 or 9, store its square in the cache */
            if (c % 10 == 1 || c % 10 == 9) {
                if (g_cache_size == g_cache_cap) {
                    g_cache_cap *= 2;
                    g_cache = (seq_t*)realloc(g_cache, g_cache_cap * sizeof(seq_t));
                }
                g_cache[g_cache_size++] = (seq_t)c * (seq_t)c;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Undefined for n = 0, but keep safe */
    ensure_cache(n);
    return g_cache[n - 1];
}
