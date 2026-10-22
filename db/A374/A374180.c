/*
 * A374180  Numbers k such that K(3 / k) != K((-1)^floor(k/2)*k / 3), where
 *          K(a/b) is the Kronecker symbol. Row 1 of A374188.
 * https://oeis.org/A374180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   kronecker_3_over_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by testing the Kronecker symbol condition
 *      K(3 / k) != K((-1)^floor(k/2) * k / 3)
 * for successive integers k.  The function returns the
 * k-th term of the sequence, where a(1)=2, a(2)=10, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/kronecker_3_over_n.h"

static seq_t *terms = NULL;      /* cached terms */
static idx_t  term_count = 0;    /* number of cached terms */
static idx_t  term_capacity = 0; /* allocated capacity */
static uint64_t next_k = 2;      /* next k to test */

/* Append a term to the cache, growing the array if needed. */
static void append_term(seq_t v)
{
    if (term_count == term_capacity) {
        size_t new_cap = term_capacity ? term_capacity * 2 : 1;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        term_capacity = new_cap;
    }
    terms[term_count++] = v;
}

/* Ensure that at least n terms have been computed. */
static void ensure_terms(idx_t n)
{
    while (term_count < n) {
        uint64_t k = next_k;

        int s1 = kronecker_3_over_n(k);

        /* compute (-1)^floor(k/2) : negative iff floor(k/2) is odd */
        bool neg = ((k >> 1) & 1ULL) != 0ULL;

        int s2;
        if (k % 3ULL == 0ULL) {
            s2 = 0;
        } else {
            int tmp = (k % 3ULL == 1ULL) ? 1 : -1; /* (k / 3) */
            s2 = neg ? -tmp : tmp;
        }

        if (s1 != s2) {
            append_term((seq_t)k);
        }

        ++next_k;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined below the offset */
    ensure_terms(n);
    return terms[n - 1];
}
