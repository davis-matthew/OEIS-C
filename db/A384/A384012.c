/*
 * A384012  a(n) = [x^n] Product_{k=0..n} (1 + k*x)^3.
 * https://oeis.org/A384012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k=0..n} (1 + kx)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Return the nth term of sequence A384012 */
/* Publicly linked function required by the build system */
seq_t a(idx_t n)
{
    /* Static buffer holding the coefficients of Q_n(x) = ∏_{k=0}^{n}(1+kx)  */
    static seq_t *b   = NULL;   /* coefficients 0 .. current_n */
    static idx_t b_cap = 0;     /* allocated size */

    /* Allocate or grow buffer if needed (C requires a cast in C++) */
    if ((idx_t)(n + 1) > b_cap) {
        seq_t *tmp = (seq_t*)realloc(b, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) abort();                   /* out of memory */
        b   = tmp;
        b_cap = n + 1;
    }

    /* Re‑initialise coefficients up to n */
    for (idx_t i = 0; i <= n; ++i) b[i] = 0;
    b[0] = 1;

    /* Build Q_n(x) = ∏_{k=0}^{n} (1 + kx) by successive multiplication */
    for (idx_t k = 1; k <= n; ++k) {
        for (idx_t t = k; t >= 1; --t) {
            b[t] += (seq_t)k * b[t - 1];
        }
    }

    /* Compute a(n) = Σ_{i+j+k=n} b[i] * b[j] * b[k] */
    seq_t result = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t bi = b[i];
        for (idx_t j = 0; j <= n - i; ++j) {
            result += bi * b[j] * b[n - i - j];
        }
    }
    return result;
}
