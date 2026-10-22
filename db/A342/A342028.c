/*
 * A342028  Numbers k such that k and k+1 both have mutually distinct
 *          exponents in their prime factorization (A130091).
 * https://oeis.org/A342028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k (with offset 1) such that * both k and
 *            k+1 have mutually distinct exponents in their prime *
 *            factorisations. The function searches sequentially for such k, *
 *            using the helper factor_u64 to obtain prime factors and
 *            exponents
 * Range:     u8 n <= 67, u16 n <= 2891, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* ------------------------------------------------------------------ */
/* Check whether a uint64_t has pairwise distinct exponents in its
 * prime factorisation.                                                    */
static bool distinct_exponents_uint64(uint64_t n) {
    if (n <= 1) return true;          /* 1 has no exponents */
    /* factor_u64 needs arrays large enough for any uint64_t: 64 is safe   */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;
    factor_u64(n, primes, exps, &cnt);
    /* exponent values for 64‑bit integers are ≤ 64 (2⁶⁴)                */
    bool seen[65] = { false };
    for (size_t i = 0; i < cnt; ++i) {
        unsigned e = (unsigned)exps[i];
        if (e <= 64) {
            if (seen[e]) return false;   /* duplicate exponent */
            seen[e] = true;
        } else {
            /* exponent cannot exceed 64 for uint64_t, but treat as failure */
            return false;
        }
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Dynamic array of seq_t values for the sequence terms                 */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Append a new term to the dynamic array.  Resizes when necessary.   */
static void add_term(seq_t v) {
    if (terms_len == terms_cap) {
        terms_cap = (terms_cap == 0) ? 128 : (terms_cap << 1);
        seq_t *tmp = seq_alloc_new_array(terms_cap);
        if (terms) {
            /* copy existing values */
            for (size_t i = 0; i < terms_len; ++i)
                tmp[i] = terms[i];
            seq_free_array(terms);
        }
        terms = tmp;
    }
    terms[terms_len++] = v;
}

/* One‑time state for the incremental search.                         */
static uint64_t last_checked = 0;

/* ------------------------------------------------------------------ */
/* Public sequence function.  It will generate terms on demand.       */
seq_t a(idx_t n) {
    /* Generate until we have at least n terms */
    while (terms_len < n) {
        ++last_checked; /* candidate k */
        /* Test k and k+1 for distinct exponents */
        if (distinct_exponents_uint64(last_checked) &&
            distinct_exponents_uint64(last_checked + 1))
        {
            add_term((seq_t)last_checked);
        }
    }
    /* Terms are stored 0‑based while a(1) is the first term   */
    return terms[n - 1];
}
