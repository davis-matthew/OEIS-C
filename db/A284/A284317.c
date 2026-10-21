/*
 * A284317  Expansion of Product_{k>=0} (1 - x^(5*k+4)) in powers of x.
 * https://oeis.org/A284317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 155, i16 n <= 417, i32 n <= 1262, i64 n <= 4187,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute the coefficient a(n) of x^n in
   ∏_{k≥0}(1 - x^(5k+4)) by iterating all factors e = 5k+4 ≤ n
   and updating an array of coefficients. */

#include <stdlib.h>

#ifdef __cplusplus
/* Allocation helpers for C++ */
static seq_t* alloc_array(size_t sz) { return new seq_t[sz]; }
static void free_array(seq_t* p) { delete[] p; }
#else
/* Allocation helpers for C */
static seq_t* alloc_array(size_t sz) { return (seq_t*)calloc(sz, sizeof(seq_t)); }
static void free_array(seq_t* p) { free(p); }
#endif

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    /* Allocate buffer for coefficients 0..n */
    size_t tot = (size_t)(n + (idx_t)1);
    seq_t *buf = alloc_array(tot);
    if (!buf)
        return (seq_t)0; /* allocation failure – unlikely to happen */

    /* Zero‑initialize buffer (calloc already does this in C; in C++ we must do it) */
    for (idx_t i = (idx_t)0; i <= n; ++i)
        buf[i] = (seq_t)0;
    buf[(idx_t)0] = (seq_t)1;

    /* Apply each factor (1 - x^e), e = 4 + 5k */
    for (idx_t e = (idx_t)4; e <= n; e += (idx_t)5)
        for (idx_t i = n; i >= e; --i)
            buf[i] -= buf[i - e];

    seq_t result = buf[n];
    free_array(buf);
    return result;
}
