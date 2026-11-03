/*
 * A255976  Primes that are the concatenation of two 3-digit primes.
 * https://oeis.org/A255976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 2753, u64 n <= 2753, u128 n <= 2753,
 *            big any n
 * Validated: full, pass, 2753 terms through n = 2753 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A255976: primes that are concatenation of two 3‑digit primes
   (computed by a sieve and search). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/mem.h"

/* Deterministic primality test for n ≤ 1 000 000 using primes ≤ 1000. */
static bool is_prime_small_uint32(uint32_t n, const uint16_t primes[], size_t primes_cnt)
{
    if (n < 2) return false;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = (uint32_t)primes[i];
        if (p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* Build all primes up to 1000. Store them sequentially in primes_out. */
static void sieve_primes_upto_1000(uint16_t primes_out[], size_t *cnt_out)
{
    /* 1000 fits in the 16‑bit type, so we use a 32‑bit loop index. */
    static const uint16_t LIMIT = 1000;
    bool is_comp[LIMIT + 1];
    memset(is_comp, 0, sizeof(is_comp));

    is_comp[0] = is_comp[1] = true;
    for (uint16_t i = 2; i * i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint16_t j = i * i; j <= LIMIT; j += i) {
                is_comp[j] = true;
            }
        }
    }

    size_t cnt = 0;
    for (uint16_t i = 2; i <= LIMIT; ++i) {
        if (!is_comp[i]) {
            primes_out[cnt++] = i;
        }
    }
    *cnt_out = cnt;
}

/* Forward declarations of helper functions used in this file */
static void build_sequence(void);
static void sieve_primes_upto_1000(uint16_t primes_out[], size_t *cnt_out);
static bool is_prime_small_uint32(uint32_t n, const uint16_t primes[], size_t primes_cnt);
static int seq_cmp(const void *p1, const void *p2);

/* Cached sequence and its length. */
static seq_t *cached_seq = NULL;
static idx_t seq_len = 0;

/* ------------------------------------------------------------------ */

/* Comparison function for qsort. */
static int seq_cmp(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

/* Build the entire sequence once and cache it. */
static void build_sequence(void)
{
    /* --------- Step 1: generate all primes ≤ 1000 --------- */
    uint16_t all_primes[200];  /* plenty of space */
    size_t all_cnt = 0;
    sieve_primes_upto_1000(all_primes, &all_cnt);

    /* --------- Step 2: enumerate pairs of 3‑digit primes --------- */
    /* There are 143 primes from 100 to 999, so at most 20449 pairs. */
    size_t max_pairs = (size_t)all_cnt * all_cnt;
    seq_t *temp = mem_new(seq_t, max_pairs);
    if (!temp) return;            /* allocation failure – leave array empty */

    size_t len = 0;
    for (size_t i = 0; i < all_cnt; ++i) {
        uint16_t pi = all_primes[i];
        if (pi < 100 || pi > 999) continue;   /* only 3‑digit primes */
        for (size_t j = 0; j < all_cnt; ++j) {
            uint16_t pj = all_primes[j];
            if (pj < 100 || pj > 999) continue;

            uint32_t val32 = (uint32_t)pi * 1000u + (uint32_t)pj;
            if (is_prime_small_uint32(val32, all_primes, all_cnt)) {
                temp[len++] = (seq_t)val32;   /* cast to seq_t for storage */
            }
        }
    }

    /* --------- Step 3: sort the collected primes --------- */
    mem_qsort(temp, len, sizeof(seq_t), seq_cmp);

    /* --------- Step 4: store in the cached array --------- */
    cached_seq = temp;
    seq_len   = (idx_t)len;
}

/* ------------------------------------------------------------------ */
/* Sequence accessor.  Indexing starts at 1 as per OEIS definition. */
seq_t a(idx_t n)
{
    if (cached_seq == NULL) {
        build_sequence();
    }
    if (n < 1 || n > seq_len) return (seq_t)0;
    return cached_seq[(size_t)(n - 1)];
}
