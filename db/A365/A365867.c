/*
 * A365867  Numbers k such that k and k+1 are both divisible by the cube of
 *          their least prime factor.
 * https://oeis.org/A365867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 732, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A365867 – numbers k such that k and k+1 are each divisible by
    the cube of their least prime factor.
    The function returns the n‑th term (a(1) = 80) and uses on‑the‑fly
    computation without hard‑coded tables.  It works for any seq_t and
    idx_t supplied by the build.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/*----- Helper: least prime factor of a 64‑bit integer ----------------*/
static uint64_t lpf_u64(uint64_t n)
{
    if (n < 2) return n;
    if ((n & 1) == 0) return 2;
    for (uint64_t p = 3; p * p <= n; p += 2) {
        if (n % p == 0) return p;
    }
    return n;           /* n is prime */
}

/*----- Cache of already generated terms -----------------------------*/
static seq_t *cache = NULL;          /* array of produced terms */
static size_t cache_size = 0;        /* number of cached terms  */
static size_t cache_cap   = 0;        /* allocated capacity      */
static uint64_t next_k    = 1;        /* next integer to test    */

/*----- Ensure the cache can hold at least 'needed' elements ----------*/
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();               /* out of memory */
    cache = new_cache;
    cache_cap = new_cap;
}

/*----- Main sequence function -------------------------------------*/
seq_t a(idx_t n)   /* n ≥ 1  */
{
    /* convert target index to a normal size_t for array access */
    if (n == 0) return (seq_t)0;  /* undefined for n=0, but harmless */
    size_t target = (size_t)n;

    /* generate terms until we have at least 'target' of them */
    while (cache_size < target) {
        uint64_t k = next_k;

        /* find the next valid term */
        while (true) {
            if (k < 2) { ++k; continue; }

            uint64_t p = lpf_u64(k);
            uint64_t q = lpf_u64(k + 1);

            if (p >= 2 && q >= 2) {
                uint64_t cube_p = p * p * p;
                uint64_t cube_q = q * q * q;
                if ((k % cube_p == 0) && ((k + 1) % cube_q == 0)) {
                    /* store the term */
                    ensure_cache_cap(cache_size + 1);
                    cache[cache_size++] = (seq_t)k;
                    next_k = k + 1;
                    break;
                }
            }
            ++k;
        }
    }

    return cache[target - 1];
}
