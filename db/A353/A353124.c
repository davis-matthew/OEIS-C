/*
 * A353124  Numbers k which have a record number of non-divisors < k (i.e.,
 *          A049820(k)).
 * https://oeis.org/A353124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_tau
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A353124: numbers k having a record number of non‑divisors < k
    (equivalently, record values of A049820(k)).  */

/*  Requirements:
    1. Static memoised cache.
    2. Divisor‑count sieve (tau(k)) rebuilt only when the limit grows.
    3. Works for any seq_t (built‑in integer types or big‑int class).
    4. Compiles under C11 and C++17.  */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_tau.h"

static seq_t *term_arr  = NULL;  /* cached record terms  */
static size_t term_len  = 0;     /* number of stored terms  */
static size_t term_cap  = 0;     /* allocated capacity of term_arr */

static uint32_t *tau_tbl = NULL; /* divisor counts up to current max_k */
static size_t     tau_cap = 0;    /* current size of tau_tbl (max_k) */

static idx_t  cur_k = 2;         /* next integer to test  */
static seq_t  max_nd = 0;        /* largest non‑divisor count seen */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n < 1 */

    /* initialise term storage once */
    if (!term_arr) {
        term_cap = 64;
        term_arr = (seq_t*)malloc(term_cap * sizeof(seq_t));
        if (!term_arr) exit(EXIT_FAILURE);
    }

    /* seed the first term if not yet present */
    if (term_len == 0) {
        term_arr[term_len++] = (seq_t)1;   /* k = 1, ND = 0 */
        max_nd = 0;
        cur_k = 2;
    }

    /* keep producing terms until we have at least n of them */
    while (term_len < (size_t)n) {
        /* Ensure divisor table covers cur_k */
        if ((size_t)cur_k > tau_cap) {
            idx_t new_limit = tau_cap ? tau_cap * 2 : cur_k;
            while ((size_t)new_limit < (size_t)cur_k) new_limit *= 2;

            uint32_t *new_tbl = build_tau(new_limit);
            free(tau_tbl);
            tau_tbl = new_tbl;
            tau_cap = new_limit;
        }

        /* compute the number of non-divisors for cur_k */
        seq_t nd = (seq_t)cur_k - (seq_t)tau_tbl[(size_t)cur_k];
        if (nd > max_nd) {
            /* new record found */
            if (term_len == term_cap) {
                term_cap *= 2;
                term_arr = (seq_t*)realloc(term_arr, term_cap * sizeof(seq_t));
                if (!term_arr) exit(EXIT_FAILURE);
            }
            term_arr[term_len++] = (seq_t)cur_k;
            max_nd = nd;
        }
        ++cur_k;
    }

    return term_arr[n - 1];
}
