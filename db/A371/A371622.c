/*
 * A371622  Primes p such that p - 2 and p + 2 have the same number of prime
 *          factors, counted with multiplicity.
 * https://oeis.org/A371622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 8887 (2026-11-03)
 * Helpers:   bigomega_seq, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A371622: primes p such that p-2 and p+2 have the same number of
 * prime factors (counted with multiplicity).  The function searches primes
 * sequentially using the helper `next_prime` and tests the condition with
 * `count_prime_factors`. */
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/bigomega_seq.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* --- dynamic array helpers ------------------------------------------- */
static void grow_seq(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 64 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, *arr, (new_cap * sizeof(**arr)) / sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* static cache for memoisation */
static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t cache_cap = 0;
static uint64_t last_checked_prime = 2;   /* last prime examined */

/* Return the n-th term (1‑based) of A371622 */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* undefined for n < 1 – return 0 */
    /* generate terms until we reach the n-th */
    while (cache_len < n) {
        uint64_t cand = next_prime(last_checked_prime + 1);
        seq_t cnt1 = bigomega_seq((seq_t)(cand - 2));
        seq_t cnt2 = bigomega_seq((seq_t)(cand + 2));
        if (cnt1 == cnt2) {
            grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = cand;
        }
        last_checked_prime = cand;   /* advance to next candidate */
    }
    return cache[n - 1];
}
