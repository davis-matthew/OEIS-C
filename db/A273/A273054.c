/*
 * A273054  Numbers n such that 19*n^2 + 20 is a square.
 * https://oeis.org/A273054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=4, a(2)=22, a(3)=1382, a(4)=7484,
   a(n)=340*a(n-2)-a(n-4) for all n>4.
   The implementation stores subsequences of odd and even indices
   separately, growing each cache dynamically on demand. */

#include <stdlib.h>

/* Cache for odd indices: odd_terms[m] = a(2*m+1)  (m >= 0) */
static seq_t *odd_terms = NULL;
static size_t odd_cap = 0;   /* allocated array size */
static size_t odd_len = 0;   /* number of valid entries */

/* Cache for even indices: even_terms[m] = a(2*m)  (m >= 1),
   even_terms[0] is unused placeholder. */
static seq_t *even_terms = NULL;
static size_t even_cap = 0;
static size_t even_len = 0;

/* Ensure odd_terms contains at least index m (m >= 0). */
static void ensure_odd(size_t m)
{
    if (odd_len > m)
        return;

    size_t old_len = odd_len;
    size_t new_cap = odd_cap > 0 ? odd_cap : 4;
    while (new_cap <= m)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(odd_terms, new_cap * sizeof(seq_t));
    odd_terms = tmp;
    odd_cap = new_cap;

    for (size_t i = old_len; i <= m; ++i) {
        if (i == 0)          /* a(1) = 4 */
            odd_terms[i] = (seq_t)4;
        else if (i == 1)     /* a(3) = 1382 */
            odd_terms[i] = (seq_t)1382;
        else                /* recurrence for odd indices */
            odd_terms[i] = (seq_t)340 * odd_terms[i - 1]
                           - odd_terms[i - 2];
    }
    odd_len = m + 1;
}

/* Ensure even_terms contains at least index m (m >= 1). */
static void ensure_even(size_t m)
{
    if (even_len > m)
        return;

    size_t old_len = even_len;
    size_t new_cap = even_cap > 0 ? even_cap : 4;
    while (new_cap <= m)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(even_terms, new_cap * sizeof(seq_t));
    even_terms = tmp;
    even_cap = new_cap;

    for (size_t i = old_len; i <= m; ++i) {
        if (i == 0) {            /* unused placeholder */
            even_terms[i] = (seq_t)0;
        }
        else if (i == 1)         /* a(2) = 22 */
            even_terms[i] = (seq_t)22;
        else if (i == 2)         /* a(4) = 7484 */
            even_terms[i] = (seq_t)7484;
        else                     /* recurrence for even indices */
            even_terms[i] = (seq_t)340 * even_terms[i - 1]
                           - even_terms[i - 2];
    }
    even_len = m + 1;
}

seq_t a(idx_t n)
{
    /* n >= 1 is guaranteed by the caller. */
    if (n & (idx_t)1) {                 /* odd index */
        size_t m = (n - 1) / 2;
        ensure_odd(m);
        return odd_terms[m];
    } else {                            /* even index */
        size_t m = n / 2;
        ensure_even(m);
        return even_terms[m];
    }
}
