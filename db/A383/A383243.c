/*
 * A383243  Primes of the form p(k)*p(k+1)*(p(k+1) - p(k)) - 1 sorted by
 *          increasing k.
 * https://oeis.org/A383243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p(k) * p(k+1) * (p(k+1)-p(k)) - 1, where p(k) is the k‑th
 *            prime, and a(1) uses k = 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 8, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

/* Static cache of primes and of the sequence terms. */
static idx_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_len = 0;

static seq_t *term_cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;

/* Index of the next prime pair to examine (0‑based). */
static idx_t next_pair_index = 0;
/* Last integer that was tested for primality. */
static idx_t last_checked_candidate = 1;

/* Append the next prime to prime_list, enlarging the array if required. */
static void add_next_prime(void)
{
    while (true) {
        ++last_checked_candidate;
        if (is_prime_small(last_checked_candidate)) {
            if (prime_len == prime_cap) {
                prime_cap = (prime_cap > 0) ? prime_cap * 2 : 4;
                prime_list = (idx_t*)realloc(prime_list,
                                            prime_cap * sizeof(idx_t));
            }
            prime_list[prime_len++] = last_checked_candidate;
            break;
        }
    }
}

/* Ensure that prime_list contains at least 'up_to' primes (1‑based index). */
static void ensure_primes(size_t up_to)
{
    while (prime_len < up_to)
        add_next_prime();
}

/* Grow term_cache if it is full. */
static void ensure_cache_capacity(void)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap > 0) ? cache_cap * 2 : 4;
        term_cache = (seq_t*)realloc(term_cache,
                                      cache_cap * sizeof(seq_t));
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* safety for n < 1 */

    /* Generate terms until the cache holds at least n values. */
    while (cache_len < (size_t)n) {
        /* At least two primes are needed to form the next pair. */
        ensure_primes(next_pair_index + 2);

        idx_t p1 = prime_list[next_pair_index];
        idx_t p2 = prime_list[next_pair_index + 1];

        seq_t diff = (seq_t)(p2 - p1);
        seq_t prod = (seq_t)p1 * (seq_t)p2 * diff - (seq_t)1;

        if (is_prime_big(prod)) {
            ensure_cache_capacity();
            term_cache[cache_len++] = prod;
        }
        ++next_pair_index;
    }

    return term_cache[n - 1];
}
