/*
 * A385660  Numbers k such that prime(k+1)-prime(k) divides k.
 * https://oeis.org/A385660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A385660
 *  Sequence definition:  a(n) = k where k is the n‑th positive integer
 *  satisfying prime(k+1) – prime(k) | k.  The function a(n) returns the
 *  value a(n) (offset 1).  It generates terms on demand and caches them
 *  for later calls.  It works for all supported build types:
 *      - 8,16,32,64‑bit unsigned integers (C)
 *      - C++ big‑integer class (C++)
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"

static idx_t *prime_arr = NULL;      /* prime_arr[0] = 2 = prime(1) */
static size_t prime_cnt = 0;         /* number of primes stored     */
static idx_t  prime_limit = 0;       /* last sieve limit used       */

static void ensure_primes_at_least(idx_t need)
{
    if (prime_cnt >= (size_t)need) return;

    while (prime_cnt < (size_t)need) {
        idx_t new_limit = prime_limit ? prime_limit * 2 : 20;
        if (prime_arr) free(prime_arr);
        build_primes(new_limit, &prime_arr, &prime_cnt);
        prime_limit = new_limit;
    }
}

static seq_t *terms   = NULL;   /* cache of found k values          */
static size_t term_cnt = 0;      /* number of terms stored           */
static size_t term_cap = 0;      /* allocated capacity                */

static void add_term(seq_t v)
{
    if (term_cnt == term_cap) {
        size_t new_cap = term_cap ? term_cap * 2 : 16;

#ifdef __cplusplus
        /* Use C++ new[]/delete[] to handle non‑trivial seq_t types */
        seq_t *tmp = new seq_t[new_cap];
        if (terms) {
            std::copy(terms, terms + term_cnt, tmp);
            delete[] terms;
        }
#else
        /* C malloc/realloc for trivial seq_t types */
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) abort();
#endif
        terms = tmp;
        term_cap = new_cap;
    }
    terms[term_cnt++] = v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 – behaviour for n<1 is irrelevant */

    static idx_t next_k = 1;        /* next k (1‑based) to test            */

    /* Generate terms until we have at least n of them */
    while (term_cnt < (size_t)n) {
        /* Acquire primes up to k+1 (prime_arr is 0‑based) */
        if (prime_cnt <= (size_t)next_k)
            ensure_primes_at_least(next_k + 1);

        idx_t pk      = prime_arr[next_k];        /* prime(k+1) */
        idx_t pk_prev = prime_arr[next_k - 1];    /* prime(k)   */
        idx_t gap = pk - pk_prev;                 /* prime gap  */

        if (gap != 0 && (next_k % gap) == 0)
            add_term((seq_t)next_k);

        ++next_k;
    }

    return terms[n - 1];   /* a(1) is stored at terms[0] */
}
