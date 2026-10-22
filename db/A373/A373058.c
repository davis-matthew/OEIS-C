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
 * Helpers:   factor_and_compute_sum
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
#include "../utils/factor_and_compute_sum.h"

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
    seq_t *tmp = (seq_t*)realloc(a_cache, new_cap * sizeof(seq_t));
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
