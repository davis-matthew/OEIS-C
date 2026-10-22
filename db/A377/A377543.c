/*
 * A377543  a(n) = least prime > a(n - 1)*a(n - 3)/a(n - 2), with a(1) = 2,
 *          a(2) = 3, a(3) = 5.
 * https://oeis.org/A377543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(2)=3, a(3)=5; a(n) = least prime >
 *            a(n-1)*a(n-3)/a(n-2) for n≥4
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big, prod_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/is_prime_big.h"
#include "../utils/prod_div.h"

/* Zero‑based cache: cache[0] holds a(1), cache[1] holds a(2), ... */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/* Ensure that the cache contains at least n terms (1‑based). */
static void ensure_cache_size(idx_t n) {
    if (cache_size >= (size_t)n)
        return;

    /* Allocate enough space; keep one slot extra to avoid off‑by‑one errors. */
    size_t need = (size_t)n + 1;
    if (cache_cap == 0)
        cache_cap = 8;
    while (cache_cap < need)
        cache_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;

    /* First allocation: initialise a(1), a(2), a(3). */
    if (cache_size == 0) {
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache_size = 3;
    }

    /* Compute terms until we reach the requested index n. */
    while (cache_size < (size_t)n) {
        seq_t a_n1 = cache[cache_size - 1];   /* a(n-1) */
        seq_t a_n3 = cache[cache_size - 3];   /* a(n-3) */
        seq_t a_n2 = cache[cache_size - 2];   /* a(n-2) */
        seq_t val   = prod_div(a_n1, a_n3, a_n2);  /* floor(a_n1*a_n3/a_n2) */

        /* Least prime greater than val. */
        seq_t cand = val + (seq_t)1;
        if (cand <= (seq_t)2) {
            cand = (seq_t)2;
        } else {
            if ((cand & (seq_t)1) == (seq_t)0)
                cand = cand + (seq_t)1;           /* make cand odd */
            while (!is_prime_big(cand))
                cand = cand + (seq_t)2;           /* skip even numbers */
        }

        cache[cache_size] = cand;
        ++cache_size;
    }
}

/* Public function: return a(n).  n is 1‑based. */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;   /* undefined for n=0 */
    ensure_cache_size(n);
    return cache[n - 1];
}
