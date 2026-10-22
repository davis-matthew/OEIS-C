/*
 * A349373  Dirichlet convolution of Kimberling's paraphrase, A003602, with
 *          Dirichlet inverse of Euler phi (A023900).
 * https://oeis.org/A349373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A003602(n/d) * A023900(d)
 * Range:     i8 n <= 261, i16 n <= 9954, i32 n <= 9954, i64 n <= 9954,
 *            i128 n <= 9954, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A003602 (Kimberling's paraphrase) is given by   f(n) = (odd_part(n)+1)/2
   where odd_part(n) is n with all factors 2 removed. */
/* A023900 is the Dirichlet inverse of Euler's totient function φ,
   computed via the recurrence     h(1)=1,
   h(n) = - Σ_{k|n, k<n} φ(n/k) * h(k)    for n>1. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/phi.h"

static seq_t *h = NULL;          /* array of h values, 1-indexed */
static idx_t h_cap = 0;          /* current capacity-1 of h (max index filled) */

/* Even though seq_t may be signed, all intermediate values in the
   recurrence stay within signed range; we rely on built‑in overflow checks. */
static void ensure_h(idx_t n)
{
    if (n < 1) return;
    if (h_cap > n) return;

    idx_t new_cap = n + 1;   /* indices 0..n inclusive (0 unused) */
    seq_t *new_h = (seq_t *)realloc(h, new_cap * sizeof(seq_t));
    if (!new_h) return;     /* allocation failure ignored – not expected */
    h = new_h;
    for (idx_t i = h_cap; i <= n; ++i) h[i] = 0;

    if (h_cap <= 1) h[1] = 1;   /* initialise base case */
    for (idx_t i = (h_cap > 1 ? h_cap : 2); i <= n; ++i) {
        seq_t sum = 0;
        /* iterate over proper divisors k of i */
        idx_t d;
        for (d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                if (d < i) {
                    seq_t term = phi(i / d) * h[d];
                    sum += term;
                }
                idx_t d2 = i / d;
                if (d2 != d && d2 < i) {
                    seq_t term = phi(i / d2) * h[d2];
                    sum += term;
                }
            }
        }
        h[i] = 0 - sum;   /* h[i] = -sum */
    }
    h_cap = new_cap;
}

static inline idx_t odd_part(idx_t x)
{
    while ((x & 1) == 0) x >>= 1;
    return x;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    ensure_h(n);            /* make sure h is ready up to n          */
    seq_t result = 0;
    idx_t d;

    for (d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            idx_t nd = n / d;
            seq_t f = ((seq_t)(odd_part(nd) + 1)) / 2;
            result += f * h[d];

            idx_t d2 = n / d;
            if (d2 != d) {          /* divisor n/d if distinct */
                idx_t nd2 = n / d2;  /* = d */
                seq_t f2 = ((seq_t)(odd_part(nd2) + 1)) / 2;
                result += f2 * h[d2];
            }
        }
    }
    return result;
}
