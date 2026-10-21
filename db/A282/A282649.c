/*
 * A282649  The larger term of the pair (a(n), a(n+1)) is always prime.
 * https://oeis.org/A282649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2, a(3)=3. * For n even (n≥4): a(n) = the
 *            (n/2+1)-th prime (p_{n/2+1}). * For n odd (n≥5): a(n) = the
 *            ((n-3)/2)-th composite (c_{(n-3)/2}). * This follows the OEIS
 *            description of A282649. * Implementation uses on‑demand caches
 *            of primes and composites
 * Range:     u8 n <= 107, u16 n <= 6489, u32 n <= 6489, u64 n <= 6489,
 *            u128 n <= 6489, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* ---------- prime cache ---------- */
static seq_t           *prime_cache  = NULL;   /* array of primes          */
static size_t          prime_cnt    = 0;       /* number of stored primes  */
static size_t          prime_cap    = 0;       /* allocated capacity       */
static idx_t           prime_next   = 2;       /* next candidate for prime*/

/* Ensure that prime_cache contains at least 'need' primes. */
static void ensure_prime_cnt(size_t need)
{
    if (prime_cnt >= need) return;

    /* Grow array if necessary. */
    if (prime_cap < need) {
        size_t new_cap = prime_cap == 0 ? 128 : prime_cap * 2;
        while (new_cap < need) new_cap *= 2;
        if (prime_cache == NULL)
            prime_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        else
            prime_cache = (seq_t*)realloc(prime_cache, new_cap * sizeof(seq_t));
        prime_cap = new_cap;
    }

    /* Generate primes until we have 'need' of them. */
    while (prime_cnt < need) {
        if (is_prime(prime_next))
            prime_cache[prime_cnt++] = (seq_t)prime_next;
        ++prime_next;
    }
}

/* ---------- composite cache ---------- */
static seq_t           *comp_cache   = NULL;   /* array of composites      */
static size_t          comp_cnt     = 0;       /* number of stored composites */
static size_t          comp_cap     = 0;       /* allocated capacity        */
static idx_t           comp_next    = 4;       /* next candidate for composite */

/* Ensure that comp_cache contains at least 'need' composites. */
static void ensure_comp_cnt(size_t need)
{
    if (comp_cnt >= need) return;

    /* Grow array if necessary. */
    if (comp_cap < need) {
        size_t new_cap = comp_cap == 0 ? 128 : comp_cap * 2;
        while (new_cap < need) new_cap *= 2;
        if (comp_cache == NULL)
            comp_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
        else
            comp_cache = (seq_t*)realloc(comp_cache, new_cap * sizeof(seq_t));
        comp_cap = new_cap;
    }

    /* Generate composites until we have 'need' of them. */
    while (comp_cnt < need) {
        if (!is_prime(comp_next) && comp_next > 1)
            comp_cache[comp_cnt++] = (seq_t)comp_next;
        ++comp_next;
    }
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Handle the three special initial terms. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    if ((n & 1U) == 0) {                     /* even index, n ≥ 4   */
        size_t prime_index = (size_t)(n / 2U + 1U);  /* 1‑based index      */
        ensure_prime_cnt(prime_index);
        return prime_cache[prime_index - 1U];
    } else {                                 /* odd index, n ≥ 5    */
        size_t comp_index = (size_t)((n - 3U) / 2U); /* 1‑based index      */
        ensure_comp_cnt(comp_index);
        return comp_cache[comp_index - 1U];
    }
}
