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
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   mem
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
#include "../utils/mem.h"

/* Compute the Kronecker symbol (3 / n) for n > 0. */
static int kronecker_3_over_n(uint64_t n)
{
    int res = 1;
    /* handle powers of two */
    unsigned twos = 0;
    while ((n & 1ULL) == 0ULL) {
        ++twos;
        n >>= 1;
    }
    if (twos & 1) res = -res;           /* (3/2)^twos : (3/2) = -1 */

    if (n % 3ULL == 0ULL) return 0;     /* (3 / 3·m) = 0 */

    /* n now odd and not divisible by 3 */
    /* extra sign from (-1)^((n-1)/2) */
    if ( ((n - 1ULL) >> 1) & 1ULL ) res = -res;

    /* (n / 3) : 1 if n≡1 mod3, -1 if n≡2 mod3 */
    int r = (n % 3ULL == 1ULL) ? 1 : -1;
    res *= r;
    return res;
}

static seq_t *terms = NULL;      /* cached terms */
static idx_t  term_count = 0;    /* number of cached terms */
static idx_t  term_capacity = 0; /* allocated capacity */
static uint64_t next_k = 2;      /* next k to test */

/* Append a term to the cache, growing the array if needed. */
static void append_term(seq_t v)
{
    if (term_count == term_capacity) {
        size_t new_cap = term_capacity ? term_capacity * 2 : 1;
        seq_t *tmp = mem_resize(seq_t, terms, new_cap);
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
