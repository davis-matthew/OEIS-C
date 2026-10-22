/*
 * A320712  Indices of primes followed by a gap (distance to next larger
 *          prime) of 28.
 * https://oeis.org/A320712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 985, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A320712: indices i such that prime(i+1) = prime(i)+28  */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"   /* uint64_t next_prime(uint64_t start); */

/* Dynamically store the found indices (1‑based prime indices). */
static seq_t *g_indices = NULL;   /* array of seq_t values */
static size_t  g_count   = 0;     /* number of stored indices */
static size_t  g_cap     = 0;     /* current capacity of g_indices */
static uint64_t g_last_prime = 0; /* last prime processed */
static idx_t   g_prime_index = 0; /* 1‑based index of g_last_prime */

/* Ensure g_indices has room for at least 'needed' elements. */
static void ensure_capacity(size_t needed) {
    if (needed <= g_cap) return;
    size_t newcap = (g_cap == 0) ? 1 : g_cap << 1;
    while (newcap < needed) newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(g_indices, newcap * sizeof(seq_t));
    if (!tmp) abort(); /* allocation failed */
    g_indices = tmp;
    g_cap = newcap;
}

/* Generate prime gaps until we have at least 'need' indices. */
static void generate_until(idx_t need) {
    /* Initialisation: first prime is 2, with index 1. */
    if (g_last_prime == 0) {
        g_last_prime = 2;
        g_prime_index = 1;
    }

    while (g_count < need) {
        /* Next prime after the current one. */
        uint64_t nxt = next_prime(g_last_prime);

        /* Check if the gap is exactly 28. */
        if (nxt - g_last_prime == 28U) {
            ensure_capacity(g_count + 1);
            g_indices[g_count] = (seq_t)g_prime_index;
            ++g_count;
        }

        /* Advance to the next prime. */
        ++g_prime_index;
        g_last_prime = nxt;
    }
}

/* Public function requested by the build. */
seq_t a(idx_t n) {
    if (n == 0U) return (seq_t)0;      /* safety, offset 1 */
    generate_until(n);
    return g_indices[n - 1U];
}
