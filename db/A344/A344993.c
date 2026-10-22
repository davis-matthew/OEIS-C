/*
 * A344993  Number of polygons formed when every pair of vertices of a row of
 *          n adjacent congruent rectangles are joined by an infinite line.
 * https://oeis.org/A344993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(n+1) + 2*Sum_{i=2..n} (n+1-i)*(2*n+2-i)*phi(i)
 * Range:     u8 n <= 4, u16 n <= 20, u32 n <= 343, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Here phi(i) is Euler's totient function. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"

static seq_t *phi_cache = NULL;           /* phi_cache[i] = φ(i) for 0 <= i < phi_len */
static size_t phi_len = 0;                /* current length of phi_cache */

/* List of primes discovered during the sieve. */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0, prime_cap = 0;

/* Ensure that phi_cache contains φ(i) for all 0 <= i <= n. */
static void ensure_phi_upto(idx_t n)
{
    if ((idx_t)phi_len >= n + 1)
        return;   /* already up to n */

    /* Allocate a larger cache. */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = seq_alloc(new_len);

    /* Copy old values. */
    for (size_t i = 0; i < phi_len; ++i)
        new_cache[i] = phi_cache[i];

    /* Initialise new entries with the identity value i. */
    for (size_t i = phi_len; i < new_len; ++i)
        new_cache[i] = (seq_t)i;

    /* Linear sieve for φ up to new_len-1. */
    for (size_t i = 2; i < new_len; ++i) {
        if (new_cache[i] == (seq_t)i) {    /* i is prime */
            new_cache[i] = (seq_t)(i - 1);
            if (prime_count == prime_cap) {
                size_t nc = prime_cap ? prime_cap * 2 : 64;
                uint64_t *tmp = (uint64_t *)realloc(prime_list, nc * sizeof(uint64_t));
                if (!tmp) abort();
                prime_list = tmp;
                prime_cap = nc;
            }
            prime_list[prime_count++] = (uint64_t)i;
        }
        for (size_t k = 0; k < prime_count; ++k) {
            uint64_t p = prime_list[k];
            size_t v = i * p;
            if (v >= new_len)
                break;
            if (i % p == 0) {
                new_cache[v] = new_cache[i] * (seq_t)p;
                break;
            } else {
                new_cache[v] = new_cache[i] * (seq_t)(p - 1);
            }
        }
    }

    /* Replace the old cache.  (Memory leak is acceptable for the lifetime.) */
    phi_cache = new_cache;
    phi_len   = new_len;
}

/* OEIS sequence A344993 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    ensure_phi_upto(n);

    seq_t sum = 0;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = (seq_t)(n + 1 - i) * (seq_t)(2 * n + 2 - i);
        term *= phi_cache[i];
        sum += term;
    }

    seq_t result = 2 * sum + 2 * (seq_t)n * (seq_t)(n + 1);
    return result;
}
