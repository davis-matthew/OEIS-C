/*
 * A360826  a(1) = 1, a(n) = (k+1)*(2*k+1), where k = Product_{i=1..n-1} a(i).
 * https://oeis.org/A360826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k + 1) * (2 * k + 1), where k = Π_{i=1}^{n-1} a(i) and
 *            a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */
#include <stdlib.h> /* for malloc, realloc, free */

#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined for n=0, safe guard */

    static idx_t count = 0;              /* number of terms computed so far */
    static seq_t product = (seq_t)1;     /* product of first 'count' terms */

#ifdef __cplusplus
    static std::vector<seq_t> terms;     /* dynamic array of terms */
#else
    static seq_t *terms = NULL;          /* dynamic array of terms (C mode) */
    static size_t capacity = 0;
#endif

    /* Return cached value if already computed */
    if (n <= count)
    {
#ifndef __cplusplus
        return terms[n-1];
#else
        return terms[n-1];
#endif
    }

    /* Compute missing terms up to n */
    while (count < n)
    {
        seq_t term;

        if (count == 0)                  /* a(1) = 1 */
        {
            term = (seq_t)1;
        }
        else
        {
            /* term = (product + 1) * (2 * product + 1) */
            term = (product + (seq_t)1)
                 * ((seq_t)2 * product + (seq_t)1);
        }

#ifdef __cplusplus
        terms.push_back(term);
#else
        /* Ensure capacity for the new term */
        if (count >= capacity)
        {
            size_t new_cap = capacity > 0 ? capacity * 2 : 4;
            seq_t *new_terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!new_terms) abort();      /* allocation failure */
            terms = new_terms;
            capacity = new_cap;
        }
        terms[count] = term;
#endif

        /* Update running product */
        product = product * term;
        ++count;
    }

    /* Return the requested term */
#ifndef __cplusplus
    return terms[n-1];
#else
    return terms[n-1];
#endif
}
