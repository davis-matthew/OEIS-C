/*
 * A336371  Numbers k such that gcd(k, prime(k) + prime(k-1)) > 1.
 * https://oeis.org/A336371
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A336371: numbers k such that gcd(k, prime(k)+prime(k-1)) > 1.
   The algorithm generates primes on demand and stores the sequence in a dynamic cache.
   It works for any seq_t (fixed‑width or big integer) and for idx_t = uint64_t.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* -------- Prime list --------------------------------------------------- */
static uint64_t *prime_list = NULL;   /* dynamic array of primes           */
static size_t   prime_cap  = 0;       /* allocated capacity               */
static size_t   prime_cnt  = 0;       /* number of primes stored          */

/* Ensure that prime_list contains at least k primes (k >= 1). */
static void ensure_prime_count(idx_t k)
{
    if (prime_cnt >= k) return;

    if (prime_list == NULL) {
        prime_cap  = 128;
        prime_list = (uint64_t*)malloc(prime_cap * sizeof(uint64_t));
        prime_cnt  = 0;
        prime_list[prime_cnt++] = 2ULL;  /* first prime */
    }

    uint64_t candidate = prime_list[prime_cnt - 1] + 1ULL;
    while (prime_cnt < k) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = prime_list[i];
            if (p * p > candidate) break;
            if (candidate % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap * 2;
                prime_list = (uint64_t*)realloc(prime_list,
                                                prime_cap * sizeof(uint64_t));
            }
            prime_list[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Return the k-th prime (1‑based). */
static uint64_t get_prime(idx_t k)
{
    ensure_prime_count(k);
    return prime_list[k - 1];
}

/* -------- Sequence cache ------------------------------------------------ */
static seq_t *seq_cache      = NULL; /* dynamic array of terms          */
static idx_t  seq_cache_len  = 0;    /* number of elements stored       */
static idx_t  seq_cache_cap  = 0;    /* allocated capacity              */
static idx_t  last_k_checked = 1;    /* last k that was examined */

/* Ensure the cache can hold at least `need` elements. */
static void grow_sequence_cache(idx_t need)
{
    if (need <= seq_cache_cap) return;
    idx_t new_cap = seq_cache_cap ? seq_cache_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache_cap = new_cap;
}

/* -------- Sequence function -------------------------------------------- */
seq_t a(idx_t n)
{
    while (seq_cache_len < n) {
        ++last_k_checked;                 /* next k to test (starting at 2) */
        uint64_t pk      = get_prime(last_k_checked);
        uint64_t pk_prev = get_prime(last_k_checked - 1);
        seq_t    sum     = (seq_t)pk + (seq_t)pk_prev;
        seq_t    g       = gcd((seq_t)last_k_checked, sum);
        if (g > 1) {
            grow_sequence_cache(seq_cache_len + 1);
            seq_cache[seq_cache_len++] = (seq_t)last_k_checked;
        }
    }
    return seq_cache[n - 1];
}
