/*
 * A322228  a(n) = [x^n] Product_{k=1..n} (k + x - k*x^2), for n >= 0.
 * https://oeis.org/A322228
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the OEIS sequence A322228:
   a(n) = [x^n] ∏_{k=1..n} (k + x - k*x^2) ,  n ≥ 0
   Recurrence:  a(n+1) = −2*(n+1)*A322227(n) + a(n)   (n ≥ 1)
   and a(n+1) = −n*(n+1)^2*A322226(n) + a(n)   (n ≥ 1)         */

/* NOTE: This implementation works in both C11 and C++17.  For C++ it
   uses new[]/delete[]; for C it falls back to malloc/realloc/free and
   assumes seq_t is a POD type.  The build system supplies the appropriate
   definitions of seq_t and idx_t.  No other libraries are used. */
#ifndef __O322228_H__
#define __O322228_H__

#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
# include <algorithm>
#endif

/* Allocation helpers */
/* ---------------------------------------------------------------------- */
#ifdef __cplusplus
static inline seq_t *alloc_seq_arr(size_t n) {
    return new seq_t[n];
}
static inline void free_seq_arr(seq_t *p, size_t /*n*/) {
    delete [] p;
}
static inline seq_t *realloc_seq_arr(seq_t *p, size_t oldn, size_t newn) {
    seq_t *old = p;
    seq_t *ne = new seq_t[newn];
    size_t copy_n = oldn < newn ? oldn : newn;
    std::copy(old, old + copy_n, ne);
    delete [] old;
    return ne;
}
#else
static inline void *alloc_seq_arr(size_t n) {
    return malloc(n * sizeof(seq_t));
}
static inline void free_seq_arr(void *p, size_t /*n*/) {
    free(p);
}
static inline seq_t *realloc_seq_arr(seq_t *p, size_t oldn, size_t newn) {
    void *np = realloc(p, newn * sizeof(seq_t));
    if (np && newn > oldn) {
        memset((seq_t*)np + oldn, 0, (newn - oldn) * sizeof(seq_t));
    }
    return (seq_t*)np;
}
#endif
/* ---------------------------------------------------------------------- */

/* Static private state for caching */
static idx_t max_computed = 0;          /* largest n processed so far */
static seq_t *coeff = NULL;             /* current polynomial coefficients */
static size_t coeff_size = 0;           /* allocated size of coeff array */
static seq_t *a_cache = NULL;           /* cached a(n) values */
static size_t a_cache_size = 0;         /* allocated size of a_cache */

/* Initialise storage for n = 0 */
static void init_storage(void) {
    if (coeff != NULL)
        return;
    coeff = alloc_seq_arr(1);   /* coeff[0] = 1 */
    coeff[0] = 1;
    coeff_size = 1;

    a_cache = alloc_seq_arr(1); /* a_cache[0] = 1 */
    a_cache[0] = 1;
    a_cache_size = 1;
}

/* Extend computation up to n, inclusive */
static void ensure_up_to(idx_t n) {
    init_storage();
    if (n <= max_computed)
        return;

    for (idx_t curr = max_computed + 1; curr <= n; ++curr) {
        /* New coefficient array size needed for degree 2*curr */
        size_t new_size = 2 * curr + 1;
        if (new_size > coeff_size) {
            coeff = realloc_seq_arr(coeff, coeff_size, new_size);
            /* if we are in C, the new elements are already zeroed by realloc_seq_arr */
            coeff_size = new_size;
        }

        /* Parameters for current step */
        const seq_t k = (seq_t)curr;
        const idx_t old_limit = 2 * (curr - 1);

        /* Update coefficients in place, high → low to avoid overwriting needed values */
        for (idx_t i = 2 * curr; ; --i) {
            seq_t old_i  = (i <= old_limit) ? coeff[i] : 0;
            seq_t old_i1 = (i >= 1 && (i - 1) <= old_limit) ? coeff[i - 1] : 0;
            seq_t old_i2 = (i >= 2 && (i - 2) <= old_limit) ? coeff[i - 2] : 0;

            coeff[i] = k * old_i + old_i1 - k * old_i2;

            if (i == 0)
                break;
        }

        /* Store the coefficient of x^curr, which is a(curr) */
        if (curr >= a_cache_size) {
            a_cache = realloc_seq_arr(a_cache, a_cache_size, curr + 1);
            a_cache_size = curr + 1;
        }
        a_cache[curr] = coeff[curr];
    }
    max_computed = n;
}

/* Public function required by the build system */
seq_t a(idx_t n) {
    ensure_up_to(n);
    return a_cache[n];
}

#endif /* __O322228_H__ */
