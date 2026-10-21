/*
 * A245315  Concatenate n-th composite number with concatenation of its prime
 *          factors in ascending order.
 * https://oeis.org/A245315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 8, u32 n <= 200, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_count, factor_u64, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) = concat( n-th composite number,
   with its prime factors in ascending order, multiplicity
   (i.e., string the composite followed by all its prime factors) )  */
/* The sequence is 1‑based: a(1) is 422 (4 = 2·2) */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* External utilities (header names follow the utils convention) */
#include "../utils/is_prime.h"       /* bool is_prime(idx_t n);          */
#include "../utils/factor_u64.h"     /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/digit_count.h"   /* uint64_t digit_count(uint64_t n);       */
#include "../utils/pow10.h"         /* seq_t pow10(idx_t e);                  */

/* Dynamic list of composite numbers.  composites[i] is the (i+1)-th composite. */
static uint64_t *composites = NULL;
static size_t   comp_len   = 0;   /* number of composites stored    */
static size_t   comp_cap   = 0;   /* allocated capacity             */

/* Extend the composites list until we have at least n composites. */
static void ensure_composites(size_t n)
{
    if (comp_len >= n)
        return;

    /* Allocate or grow the array as necessary. */
    if (comp_cap < n) {
        size_t newcap = comp_cap ? comp_cap * 2 : 16;
        if (newcap < n) newcap = n;
        uint64_t *tmp = (uint64_t *)realloc(composites, newcap * sizeof(uint64_t));
        if (!tmp) exit(EXIT_FAILURE);
        composites = tmp;
        comp_cap   = newcap;
    }

    /* Find composites starting after the last one we know. */
    uint64_t candidate = (comp_len == 0) ? 4 : composites[comp_len - 1] + 1;

    while (comp_len < n) {
        if (!is_prime((idx_t)candidate))
            composites[comp_len++] = candidate;
        ++candidate;
    }
}

/* Sequence value a(n).  n >= 1 for defined values; n == 0 returns 0. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* safe fallback for off‑by‑one misuse */

    /* Build all composites up to the n-th one. */
    ensure_composites((size_t)n);
    uint64_t composite = composites[(size_t)n - 1];

    /* Factorise the composite.  The routine returns primes in ascending order. */
    uint64_t pr[32], ex[32];
    size_t cnt = 0;
    factor_u64(composite, pr, ex, &cnt);

    /* Concatenate the composite with its prime factors. */
    seq_t result = (seq_t)composite;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        for (uint64_t j = 0; j < e; ++j) {
            seq_t mult = pow10((idx_t)digit_count(p));
            result = result * mult + (seq_t)p;
        }
    }
    return result;
}
