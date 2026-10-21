/*
 * A289601  a(n) is the number of permutations of length n that avoid the
 *          pattern 321 and the mesh pattern (12, 431) or the same sequence
 *          for the mesh pattern (12, 491).
 * https://oeis.org/A289601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0, 1, 2 a(n) = C_n - C_{n-2} - (n - 2) for n ≥
 *            3 where C_k is the k‑th Catalan number. C_0 = 1, C_{k} = C_{k-1}
 *            * (4*k - 2) / (k + 1) (k ≥ 1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* static cache for Catalan numbers C[0 ..] */
static seq_t *cat = NULL;      /* array of Catalan numbers */
static size_t cat_cap = 0;     /* allocated capacity of 'cat' */
static idx_t  cat_sz = 0;      /* number of computed Catalan terms  (0 .. cat_sz-1) */

seq_t a(idx_t n)
{
    /* a(0), a(1), a(2) are all 1 */
    if (n < 3)
        return (seq_t)1;

    /* make sure we have Catalan numbers up to n */
    if ((idx_t)cat_sz <= n) {
        /* need capacity at least n+1 */
        size_t need = (size_t)n + 1;
        if (cat_cap < need) {
            size_t new_cap = cat_cap ? cat_cap << 1 : 1;
            while (new_cap < need)
                new_cap <<= 1;
            /* realloc with explicit cast for C++ */
            cat = (seq_t *)realloc(cat, new_cap * sizeof(seq_t));
            cat_cap = new_cap;
        }
        /* compute missing Catalan numbers */
        for (idx_t i = cat_sz; i <= n; ++i) {
            if (i == 0) {
                cat[0] = (seq_t)1;
            } else {
                seq_t prev = cat[(size_t)(i - 1)];
                seq_t mult = (seq_t)(4 * i - 2);      /* (4*i - 2) */
                seq_t next = (prev * mult) / (seq_t)(i + 1);
                cat[(size_t)i] = next;
            }
        }
        cat_sz = n + 1;
    }

    /* a(n) = C_n - C_{n-2} - (n-2)  for n >= 3 */
    seq_t res = cat[(size_t)n] - cat[(size_t)(n - 2)];
    res -= ((seq_t)n - 2);
    return res;
}
