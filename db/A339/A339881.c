/*
 * A339881  Fundamental nonnegative solution x(n) of the Diophantine equation
 *          x^2 - A045339(n)*y^2 = -2, for n >= 1.
 * https://oeis.org/A339881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 29, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computing A339881:
   Fundamental non‑negative solution x of x² – p·y² = –2
   where p is the n‑th prime ≡ 2 or 3 (mod 8). */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

static seq_t *cache        = NULL; /* computed a(n) values */
static idx_t  cache_cap    = 0;
static idx_t  cache_cnt    = 0;     /* number of filled entries */

/* Prime – only odd primes are stored in primes_all. */
static idx_t *primes_all   = NULL;
static size_t primes_all_cap = 0;
static idx_t  primes_all_cnt = 0;

/* Primes ≡ 2 or 3 (mod 8) in order. */
static seq_t *prime_form   = NULL;
static size_t prime_form_cap = 0;
static idx_t  prime_form_cnt = 0;

/* Next odd integer to test for primality. */
static idx_t next_candidate = 3;

/* ------------------------------------------------------------------ */
/* Utility: grow an array of seq_t elements. */
static void grow_seq_t_arr(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t nc = (*cap == 0) ? 8 : (*cap << 1);
        while (nc < needed) nc <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, nc * sizeof(seq_t));
        *arr = tmp;
        *cap = nc;
    }
}

/* Utility: grow an array of idx_t elements. */
static void grow_idx_arr(idx_t **arr, size_t *cap, size_t needed)
{
    if (*cap < needed) {
        size_t nc = (*cap == 0) ? 8 : (*cap << 1);
        while (nc < needed) nc <<= 1;
        idx_t *tmp = (idx_t *)realloc(*arr, nc * sizeof(idx_t));
        *arr = tmp;
        *cap = nc;
    }
}

/* ------------------------------------------------------------------ */
/* Ensure that prime_form contains at least target primes with
   p ≡ 2 or 3 (mod 8).  During this process primes_all is also
   filled for primality tests. */
static void ensure_prime_form(idx_t target)
{
    if (prime_form_cnt >= target) return;

    /* First initialisation for the very first call. */
    if (prime_form_cnt == 0) {
        grow_idx_arr(&primes_all, &primes_all_cap, 1);
        grow_seq_t_arr(&prime_form, &prime_form_cap, 1);
        primes_all[0] = 2;
        primes_all_cnt = 1;
        prime_form[0] = (seq_t)2;
        prime_form_cnt = 1;
    }

    /* Generate primes until we have target entries in prime_form. */
    while (prime_form_cnt < target) {
        idx_t cand = next_candidate;
        next_candidate += 2;                     /* skip even numbers */

        bool is_prime = true;
        for (size_t i = 0; i < primes_all_cnt &&
             ((idx_t)primes_all[i] * (idx_t)primes_all[i] <= cand); ++i) {
            if (cand % primes_all[i] == 0) { is_prime = false; break; }
        }

        if (is_prime) {
            grow_idx_arr(&primes_all, &primes_all_cap,
                         primes_all_cnt + 1);
            primes_all[primes_all_cnt++] = cand;

            if (cand == 2 || cand % 8 == 3) {
                grow_seq_t_arr(&prime_form, &prime_form_cap,
                               prime_form_cnt + 1);
                prime_form[prime_form_cnt++] = (seq_t)cand;
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Compute the fundamental solution of x² – p·y² = –2 for p ≥ 2 and
   p ≡ 2 or 3 (mod 8).  p equal to 2 has the trivial solution (0,1).  */
static seq_t compute_solution(seq_t p)
{
    if (p == (seq_t)2) return (seq_t)0;

    /* Integer part of √p */
    seq_t a0 = floor_sqrt(p);
    seq_t m = 0, d = 1, a = a0;

    /* Convergents of continued fraction of √p.   */
    seq_t p_nm2 = 0, p_nm1 = 1;
    seq_t q_nm2 = 1, q_nm1 = 0;

    for (;;) {
        seq_t p_val = a * p_nm1 + p_nm2;
        seq_t q_val = a * q_nm1 + q_nm2;

        /* Test equation: p_val² – p·q_val² == –2 ? */
        if ((p_val * p_val) - p * (q_val * q_val) == (seq_t)-2)
            return p_val;

        /* Next step of the continued‑fraction algorithm. */
        seq_t m_next = d * a - m;
        seq_t d_next = (p - m_next * m_next) / d;
        seq_t a_next = (a0 + m_next) / d_next;

        /* Shift the convergent indices for next iteration. */
        p_nm2 = p_nm1;  p_nm1 = p_val;
        q_nm2 = q_nm1;  q_nm1 = q_val;
        m = m_next; d = d_next; a = a_next;
    }
}

/* ------------------------------------------------------------------ */
/* Main sequence function.  a(1) corresponds to the first prime
   2, a(2) to the next prime 3, etc.  The result is computed lazily
   and cached so that repeated calls are O(1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n<1, return 0 */

    /* Allocate or grow the cache on the first call. */
    if (cache == NULL) {
        grow_seq_t_arr(&cache, &cache_cap, n);
        cache_cnt = 0;
    }

    /* Ensure sufficient capacity. */
    if (cache_cap < n) grow_seq_t_arr(&cache, &cache_cap, n);

    /* Compute missing values up to index n. */
    while (cache_cnt < n) {
        idx_t k = cache_cnt;                  /* zero‑based index */
        ensure_prime_form(k + 1);              /* need k+1 primes in prime_form */
        seq_t p = prime_form[k];
        cache[cache_cnt++] = compute_solution(p);
    }

    return cache[n - 1];
}
