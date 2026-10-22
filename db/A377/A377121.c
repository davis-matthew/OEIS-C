/*
 * A377121  Numbers whose totient is refactorable.
 * https://oeis.org/A377121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_spf, calc_phi, divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose totient is refactorable. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/calc_phi.h"
#include "../utils/divisor_count_u64.h"

static seq_t  *terms      = NULL;   /* array of found terms         */
static size_t  terms_len  = 0;       /* number of terms stored       */
static size_t  terms_cap  = 0;       /* current capacity of terms    */
static idx_t  *spf        = NULL;   /* smallest prime factor table  */
static idx_t   spf_limit  = 0;      /* limit used for spf array     */
static idx_t   max_n_seen = 0;       /* largest n examined so far    */

/* Ensure that the first `target` terms of the sequence have been found. */
static void ensure_terms(idx_t target)
{
    /* Nothing to do if we already have enough terms. */
    if (terms_len >= (size_t)target)
        return;

    /* Growing strategy: enlarge the sieve limit and scan new numbers.
       The new limit is chosen large enough to cover the expected
       density of terms (~½). */
    idx_t new_limit = (max_n_seen + 1 < target * 4) ? target * 4 : max_n_seen + 1;
    if (new_limit < 200) new_limit = 200;          /* minimal size for safety */

    /* Rebuild the SPF table for the new limit. */
    if (spf)
        free(spf);
    build_spf(new_limit, &spf);
    spf_limit = new_limit;

    /* Ensure the terms array has enough capacity. */
    if (terms_cap == 0) {
        terms_cap = target + 10;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        terms_len = 0;
    } else if (terms_cap < terms_len + 10) {
        terms_cap *= 2;
        terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
    }

    /* Scan integers from (max_n_seen+1) up to new_limit. */
    for (idx_t n = max_n_seen + 1; n <= new_limit; ++n) {
        seq_t phi = calc_phi((seq_t)n);

        /* phi fits in 64 bits for our search range. */
        uint64_t phi_u64 = (uint64_t)phi;

        seq_t tau_seq = divisor_count_u64(phi_u64);
        uint64_t tau = (uint64_t)tau_seq;

        if (phi % (seq_t)tau == 0) {            /* phi is refactorable */
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
            }
            terms[terms_len++] = (seq_t)n;
        }
    }

    max_n_seen = new_limit;
    /* If still insufficient, recurse (this will rebuild a larger sieve). */
    if (terms_len < (size_t)target)
        ensure_terms(target);
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                               /* undefined for offset 0 */
    ensure_terms(n);
    return terms[(size_t)(n - 1)];               /* a(1) is at index 0 */
}
