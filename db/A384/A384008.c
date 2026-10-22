/*
 * A384008  Irregular triangle read by rows where row n lists the first
 *          differences of the 0-prepended prime indices of the n-th
 *          squarefree number.
 * https://oeis.org/A384008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   alloc_seq_array, build_primes, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A384008
    Irregular triangle read by rows where row n lists the first differences
    of the 0‑prepended prime indices of the n‑th squarefree number.
    The function generates squarefree numbers in order, collects the
    prime‑index differences for each, and concatenates them into the
    output sequence.  The implementation is fully memoised and works
    correctly for any n ≥ 1.  All sequence values are computed in seq_t
    without overflow (multiplications are performed in smaller types). */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) -> seq_t *   */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t *)              */
#include "../utils/build_primes.h"      /* build_primes(limit, &primes, &cnt)   */

/* Global state: flattened flattened triangle values. */
static seq_t *seq      = NULL;
static size_t seq_size = 0;  /* number of elements stored */
static size_t seq_cap  = 0;  /* allocated capacity */

/* Auxiliary sieve data for smallest prime factor and prime index. */
static uint32_t *spf       = NULL;
static uint32_t *prime_idx = NULL;
static size_t   spf_limit  = 0;   /* maximum integer for which spf is built */

/* Next integer to test for square‑freeness (starts at 2). */
static idx_t next_candidate = 2;

/* Ensure seq array has capacity for at least `new_size` elements. */
static void ensure_seq_capacity(size_t new_size)
{
    if (new_size <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < new_size) new_cap <<= 1;
    seq_t *new_arr = alloc_seq_array(new_cap);
    for (size_t i = 0; i < seq_size; ++i) new_arr[i] = seq[i];
    if (seq) free_seq_array(seq);
    seq      = new_arr;
    seq_cap  = new_cap;
}

/* Build smallest prime factor array and prime‑index mapping up to `limit`. */
static void build_spf(idx_t limit)
{
    if ((size_t)limit <= spf_limit) return;            /* already enough */

    size_t n = (size_t)limit;
    uint32_t *new_spf = (uint32_t*)calloc(n + 1, sizeof(uint32_t));
    uint32_t *new_idx = (uint32_t*)calloc(n + 1, sizeof(uint32_t));

    /* Linear sieve */
    size_t *primes = NULL;
    size_t prime_cnt = 0, prime_cap = 0;

    for (size_t i = 2; i <= n; ++i) {
        if (new_spf[i] == 0) {                 /* i is prime */
            new_spf[i] = (uint32_t)i;
            if (prime_cnt == prime_cap) {
                prime_cap = prime_cap ? prime_cap * 2 : 16;
                primes = (size_t*)realloc(primes, prime_cap * sizeof(size_t));
            }
            primes[prime_cnt++] = i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            size_t p = primes[j];
            if (p > new_spf[i] || i * p > n) break;
            new_spf[i * p] = (uint32_t)p;
        }
    }

    /* Map every prime to its 1‑based index. */
    for (size_t i = 0; i < prime_cnt; ++i)
        new_idx[primes[i]] = (uint32_t)(i + 1);

    free(primes);
    free(spf);
    free(prime_idx);
    spf        = new_spf;
    prime_idx  = new_idx;
    spf_limit  = n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safe default, not used by OEIS */

    /* Fast return if already computed. */
    if ((size_t)n <= seq_size) return seq[n - 1];

    /* Generate rows until we have at least n terms. */
    while ((size_t)n > seq_size) {

        /* Ensure sieve covers current candidate. */
        if ((size_t)next_candidate > spf_limit) {
            idx_t new_limit = next_candidate * 2ULL;
            build_spf(new_limit);
        }

        /* Factor next_candidate and collect distinct prime indices. */
        uint32_t idxs[64];
        size_t k = 0;
        idx_t tmp = next_candidate;
        bool ok = true;
        uint32_t last_p = 0;

        while (tmp > 1 && ok) {
            uint32_t p = spf[(size_t)tmp];
            if (p == last_p) {          /* repeated factor → not square‑free */
                ok = false;
                break;
            }
            if (k < sizeof(idxs) / sizeof(idxs[0]))
                idxs[k++] = prime_idx[p];
            last_p = p;
            tmp /= p;
        }

        if (ok && tmp == 1 && k > 0) {   /* valid square‑free number */
            uint32_t prev = 0;
            for (size_t i = 0; i < k; ++i) {
                uint32_t diff = idxs[i] - prev;
                ensure_seq_capacity(seq_size + 1);
                seq[seq_size++] = diff;
                prev = idxs[i];
            }
        }

        /* Advance to next integer.  The increment occurs after full row
           processing, even if seq_size already reached n. This guarantees
           that all diffs of a row are stored for future calls. */
        ++next_candidate;
    }

    return seq[n - 1];
}
