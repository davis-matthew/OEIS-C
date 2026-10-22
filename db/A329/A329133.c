/*
 * A329133  Numbers whose augmented differences of prime indices are an
 *          aperiodic sequence.
 * https://oeis.org/A329133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   build_primes, build_spf, seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A329133: Numbers whose augmented differences of prime indices are aperiodic.
   The function generates the sequence term a(n) by testing each integer for the
   aperiodicity property defined in the OEIS entry. */

#include <stdbool.h>
#include <stdlib.h>

/* ---------- Include helper routines from ../utils ----------
   (The build system will provide the required headers.) ----------
*/
#include "../utils/build_primes.h"      /* build_primes */
#include "../utils/build_spf.h"         /* build_spf */
#include "../utils/seq_alloc.h"         /* seq_alloc */

/* ---------- Static cache for generated sequence ----------
   seq   : array of accepted numbers
   len   : number of terms generated so far
   cap   : allocated capacity of the array
   next  : next integer to test
*/
static seq_t *seq   = NULL;
static size_t len   = 0;
static size_t cap   = 0;
static idx_t  next  = 1;

/* ---------- Prime sieve and factoring support ----------
   prime_list : array of primes up to spf_limit
   prime_cnt  : number of primes in prime_list
   spf        : smallest prime factor array
   spf_limit  : maximum integer for which spf is valid (inclusive)
*/
static idx_t  *prime_list  = NULL;
static size_t  prime_cnt   = 0;
static idx_t  *spf         = NULL;
static idx_t   spf_limit   = 0;

/* Grow the prime data structures to cover all integers up to 'limit'.
   The function uses build_spf and build_primes from the utils. */
static void ensure_prime_info(idx_t limit)
{
    if (limit <= spf_limit) return;  /* already available */

    /* Free old structures if they exist. */
    if (spf)           free(spf);
    if (prime_list)    free(prime_list);

    /* Build new sieve and prime list. */
    build_spf(limit, &spf);
    build_primes(limit, &prime_list, &prime_cnt);

    spf_limit = limit;
}

/* Find the 1‑based index of prime p in the sorted prime list.
   It is assumed that p is prime and <= spf_limit. */
static idx_t prime_index(idx_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo should point to p */
    return (idx_t)(lo + 1);
}

/* Check whether an integer n has a period‑free augmented difference sequence. */
static bool is_aperiodic(idx_t n)
{
    /* Arrays for the prime indices and for the augmented differences.
       The maximum number of prime factors (with multiplicity) of a 64‑bit integer
       is well below 64, so fixed‑size arrays are sufficient. */
    static idx_t indices[64];
    static idx_t w[64];
    size_t k = 0;            /* number of indices collected */

    /* Factor n using the smallest‑prime‑factor sieve. */
    idx_t m = n;
    while (m > 1) {
        idx_t p   = spf[(size_t)m];
        idx_t idx = prime_index(p);

        /* Push idx once for each occurrence of p. */
        while (m % p == 0) {
            indices[k++] = idx;
            m /= p;
        }
    }

    /* The empty set (n = 1) is considered aperiodic. */
    if (k == 0) return true;

    /* Sort indices in non‑increasing order (descending). */
    for (size_t i = 1; i < k; ++i) {
        for (size_t j = i; j > 0 && indices[j-1] < indices[j]; --j) {
            idx_t tmp = indices[j];
            indices[j] = indices[j-1];
            indices[j-1] = tmp;
        }
    }

    /* Compute augmented differences. */
    for (size_t i = 0; i + 1 < k; ++i)
        w[i] = indices[i] - indices[i+1] + 1;
    w[k-1] = indices[k-1];

    /* Test aperiodicity: check all non‑trivial cyclic rotations. */
    for (size_t shift = 1; shift < k; ++shift) {
        bool same = true;
        for (size_t i = 0; i < k; ++i) {
            if (w[(i + shift) % k] != w[i]) {
                same = false;
                break;
            }
        }
        if (same) return false;   /* period found */
    }
    return true;   /* no period */
}

/* Ensure that the sequence cache contains at least 'need' terms. */
static void ensure_sequence_upto(size_t need)
{
    while (len < need) {
        /* Expand prime tables if necessary. */
        ensure_prime_info(next);

        /* Test current candidate. */
        if (is_aperiodic(next)) {
            /* Grow the sequence array if needed. */
            if (len + 1 > cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                seq = tmp;
                cap = new_cap;
            }
            seq[len++] = (seq_t)next;
        }
        ++next;
    }
}

/* Public entry point.  The index n is 1‑based (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined behavior, return 0 */
    ensure_sequence_upto((size_t)n);
    return seq[(size_t)n - 1];
}
