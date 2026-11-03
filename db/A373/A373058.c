/*
 * A373058  The sum of the aliquot coreful divisors of the nonsquarefree
 *          numbers.
 * https://oeis.org/A373058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9348 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A373058
 *  a(n) = sum of aliquot coreful divisors of the nth nonsquarefree number
 *  (definition followed directly from the OEIS description).
 *
 *  The implementation uses a simple trial‑division factorisation and
 *  incremental caching so that any n ≥ 1 can be read arbitrarily.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/*  compute sum of coreful divisors of x (excluding x) and whether x
 *  is nonsquarefree.  The function returns the sum; the out_nonsq flag
 *  indicates if x has an exponent ≥ 2 (i.e. is nonsquarefree).               */
static seq_t factor_and_compute_sum(uint64_t x, bool *out_nonsq)
{
    seq_t prod = 1;
    bool nonsq = false;
    uint64_t t = x;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            int   exp = 0;
            seq_t pe  = 1;   /* p^exp */
            seq_t Si  = 0;   /* Σ_{k=1}^{exp} p^k */
            while (t % p == 0) {
                t   /= p;
                exp++;
                pe  *= (seq_t)p;
                Si  += pe;
                if (exp >= 2) nonsq = true;
            }
            prod *= Si;
        }
    }
    if (t > 1) {              /* remaining prime factor with exponent 1 */
        prod *= (seq_t)t;     /* Σ_{k=1}^{1} t^k = t */
    }
    *out_nonsq = nonsq;
    return prod - (seq_t)x;   /* exclude the number itself (aliquot sum) */
}

/* static state ----------------------------------------------------------*/

static seq_t *a_cache = NULL;        /* cache of computed a(n) values */
static size_t  a_cache_cap = 0;      /* allocated capacity of a_cache */
static size_t  a_cache_len = 0;      /* number of cached entries */

static uint64_t next_candidate = 1;  /* next integer to test for nonsquarefree */
static idx_t    non_sq_count   = 0;  /* count of nonsquarefree numbers seen */

/* ---------------------------------------------------------------------*/

/* ---------------------------------------------------------------------*/
/*  ensure that a_cache has room for at least `need` elements            */
static void ensure_cache(size_t need)
{
    if (need <= a_cache_cap) return;
    size_t new_cap = a_cache_cap ? a_cache_cap * 2ULL : 16ULL;
    if (new_cap < need) new_cap = need;
    seq_t *tmp = mem_resize(seq_t, a_cache, new_cap);
    if (!tmp) abort();        /* out of memory in both C and C++ */
    a_cache = tmp;
    a_cache_cap = new_cap;
}

/* ---------------------------------------------------------------------*/
/*  sequence function ----------------------------------------------------*/
seq_t
a(idx_t n)
{
    if (n == 0) return 0;      /* undefined; return 0 for safety */

    /* generate nonsquarefree numbers until we have the nth one */
    while (non_sq_count < n) {
        next_candidate++;
        bool nonsq = false;
        seq_t val  = factor_and_compute_sum(next_candidate, &nonsq);
        if (nonsq) {
            non_sq_count++;
            ensure_cache(a_cache_len + 1);
            a_cache[a_cache_len++] = val;
        }
    }

    /* cache indices are 0‑based, input indices are 1‑based   */
    return a_cache[n - 1];
}
