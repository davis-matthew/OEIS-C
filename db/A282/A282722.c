/*
 * A282722  Let p = n-th prime == 3 mod 8; a(n) = sum of quadratic residues
 *          mod p that are > p/2.
 * https://oeis.org/A282722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of quadratic residues mod p that are > p/2, where p
 *            is the n-th prime ≡ 3 (mod 8)
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 951 (2026-11-03)
 * Helpers:   mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"
#include "../utils/mem.h"

/* helper: compute sum of residues > p/2 for a single prime p */
static seq_t
sum_residues_gt_half(uint64_t p)
{
    seq_t sum = 0;
    seq_t sq   = 1;                    /* t^2 mod p for t = 1 */
    seq_t p_half = (seq_t)(p / 2);     /* p/2  (integer division) */
    uint64_t m = (p - 1) / 2;          /* number of t values */

    for (uint64_t t = 1; t <= m; ++t) {
        if (sq > p_half)
            sum += sq;

        if (t != m) {                 /* at last step no need to advance */
            sq += (seq_t)(2 * t + 1);  /* (t+1)^2 = t^2 + 2t + 1 */
            if (sq >= p)
                sq -= p;
        }
    }
    return sum;
}

/* main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* offset is 1; behaviour below is irrelevant */

    /* static cache for primes of the form 3 (mod 8) */
    static uint64_t           *prime_arr   = NULL;
    static size_t              prime_cnt   = 0;
    static size_t              prime_cap   = 0;

    /* static cache for computed a(k) values */
    static seq_t              *a_cache     = NULL;
    static size_t              a_cache_len = 0;
    static size_t              a_cache_cap = 0;

    /* ensure prime cache large enough for n-th such prime */
    while (prime_cnt < n) {
        uint64_t start = (prime_cnt == 0) ? 2 : prime_arr[prime_cnt - 1] + 1;
        uint64_t p;
        do {
            p = next_prime(start);
            start = p + 1;
        } while ((p & 7) != 3);   /* p % 8 == 3 */

        if (prime_cnt >= prime_cap) {
            size_t new_cap = prime_cap ? prime_cap * 2 : 64;
            uint64_t *tmp = (uint64_t *)realloc(prime_arr, new_cap * sizeof(uint64_t));
            if (!tmp) abort();
            prime_arr = tmp;
            prime_cap = new_cap;
        }
        prime_arr[prime_cnt++] = p;
    }

    /* ensure a_cache large enough for n values */
    if (n > a_cache_cap) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 64;
        if (n > new_cap)
            new_cap = n;
        seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
        if (!tmp) abort();
        a_cache = tmp;
        a_cache_cap = new_cap;
    }

    /* compute and cache any missing a(k) up to n */
    for (size_t i = a_cache_len; i < (size_t)n; ++i) {
        a_cache[i] = sum_residues_gt_half(prime_arr[i]);
    }
    a_cache_len = n;

    return a_cache[n - 1];
}
