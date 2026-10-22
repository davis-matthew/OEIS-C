/*
 * A365799  Parity of the Dirichlet inverse of the characteristic function of
 *          triangular numbers (A010054).
 * https://oeis.org/A365799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000035( A365800(n) )
 * Range:     u8 n <= 120, u16 n <= 9690, u32 n <= 9690, u64 n <= 9690,
 *            u128 n <= 9690, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The Dirichlet inverse f of the characteristic function of triangular
   numbers is computed modulo 2 (parity).  f is defined by
   f(1)=1 and for n>1
   f(n) = sum_{d|n, d>1} c(d) * f(n/d)   (mod 2),
   where c(d)=1 iff d is a triangular number.
   a(n) is the parity of f(n), i.e. 0 or 1. */

#include <stdlib.h>
#include <stddef.h>

static seq_t *f_cache = NULL;              /* parity of f(n) */
static unsigned char *tri_cache = NULL;    /* 1 if index is triangular */
static size_t f_size = 0;                  /* current allocation size */

/* Allocate a new array, resizing if necessary.  Works in C and C++. */
static void *resize_array(void *old, size_t old_count, size_t new_count,
                          size_t elem_size)
{
    if (old == NULL) {
#ifdef __cplusplus
        return new char[new_count * elem_size];
#else
        return malloc(new_count * elem_size);
#endif
    }
#ifdef __cplusplus
    void *newptr = realloc(old, new_count * elem_size);
#else
    void *newptr = realloc(old, new_count * elem_size);
#endif
    return newptr;
}

/* Ensure that f_cache and tri_cache are large enough for indices up to n. */
static void ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;
    if (required <= f_size)
        return;

    /* resize f_cache */
    void *new_f = resize_array(f_cache, f_size, required,
                               sizeof(seq_t));
    if (!new_f) return; /* out of memory; silently ignore */

#ifdef __cplusplus
    f_cache = static_cast<seq_t*>(new_f);
#else
    f_cache = (seq_t*)new_f;
#endif

    /* resize tri_cache */
    void *new_tri = resize_array(tri_cache, f_size, required,
                                 sizeof(unsigned char));
    if (!new_tri) return;

#ifdef __cplusplus
    tri_cache = static_cast<unsigned char*>(new_tri);
#else
    tri_cache = (unsigned char*)new_tri;
#endif

    /* initialize new entries */
    for (size_t i = f_size; i < required; ++i) tri_cache[i] = 0;

    /* mark all triangular numbers up to required-1 */
    for (idx_t k = 1;; ++k) {
        seq_t t = (seq_t)k * (k + 1) / 2;
        if (t >= required) break;
        tri_cache[(size_t)t] = 1;
    }

    /* set base value f(1)=1 if not already set */
    if (f_size < 2) {
        f_cache[0] = 0; /* unused */
        f_cache[1] = (seq_t)1;
    }

    /* compute missing f values */
    for (idx_t m = (size_t)f_size; m <= n; ++m) {
        if (m <= 1) {
            f_cache[m] = (seq_t)m; /* f(0)=0, f(1)=1 */
            continue;
        }
        unsigned int parity = 0;

        /* divisor d = m itself (if triangular) */
        if (tri_cache[(size_t)m])
            parity ^= 1;                    /* f(1)=1 */

        /* proper divisors d > 1, d <= sqrt(m) */
        for (idx_t d = 2; d * d <= m; ++d) {
            if (m % d) continue;
            idx_t e = m / d;  /* e >= d */

            if (tri_cache[(size_t)d])
                parity ^= (unsigned int)f_cache[(size_t)e];

            if (e != d && e > 1) {
                if (tri_cache[(size_t)e])
                    parity ^= (unsigned int)f_cache[(size_t)d];
            }
        }
        f_cache[m] = (seq_t)(parity & 1);
    }

    f_size = required;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined below offset */
    ensure_cache(n);
    return f_cache[(size_t)n];
}
