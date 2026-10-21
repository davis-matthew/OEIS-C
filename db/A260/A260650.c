/*
 * A260650  G.f. satisfies: A(x)^2 = A( x^2/(1-4*x)^2 ).
 * https://oeis.org/A260650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A260650
 *
 * G.f. satisfies:  A(x)^2 = A( x^2/(1-4*x)^2 )
 *
 * Recurrence (for n ≥ 2):
 *
 *     2*a(n) = R(n+1) – S(n)
 *
 * where
 *
 *     R(m) = Σ_{k=1}^{⌊m/2⌋} a(k) * C(m-1, 2k-1) * 4^{m-2k}
 *     S(n) = Σ_{i=2}^{n-1} a(i) * a(n+1-i)
 *
 * and a(1) = 1.
 *
 * The function below implements exactly this recurrence and memoises
 * previously computed values.  It works for any seq_t/idx_t pair that
 * supports the arithmetic operators used here.
 */

#include <stdlib.h>
#include <stdint.h>

/* ----- Memoisation storage ----- */
static seq_t *seq      = NULL; /* array of a(n), 1‑based */
static size_t seq_cap  = 0;    /* allocated capacity */
static idx_t  seq_len  = 0;    /* largest index computed (≥ 0) */

/* ensure that the array has space for indices up to n (0‑based index n+1) */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 1;
    while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    for (size_t i = seq_cap; i < new_cap; ++i) tmp[i] = 0;
    seq = tmp;
    seq_cap = new_cap;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out of range – not used */

    ensure_capacity(n);
    if (seq_len >= n) return seq[n];

    /* initialise the very first term if needed */
    if (seq_len == 0) {
        seq[1] = 1;                 /* a(1) = 1 */
        seq_len = 1;
    }

    /* compute all missing terms up to n */
    for (idx_t cur = seq_len + 1; cur <= n; ++cur) {

        idx_t m = cur + 1;                   /* we need R(m) with m = cur+1 */
        idx_t max_k = m / 2;                 /* floor((cur+1)/2) */

        /* ----------- compute R(m) ----------- */
        seq_t R = 0;

        /* 4^{cur-1}  (exponent for the first k=1) */
        seq_t pow4 = 1;
        for (idx_t i = 0; i < cur - 1; ++i)      /* cur-1 multiplications */
            pow4 *= (seq_t)4;

        /* first binomial C(m-1,1) = m-1 */
        seq_t C = (seq_t)(m - 1);
        idx_t r = 1;                            /* current r = 2k-1 */

        for (idx_t k = 1; k <= max_k; ++k) {
            /* term = a[k] * C * pow4 */
            seq_t term = seq[k];
            term *= C;
            term *= pow4;
            R   += term;

            if (k < max_k) {
                /* advance C to C(m-1, r+2) using
                   C' = C * (m-1-r)*(m-1-r-1) / ((r+1)*(r+2))
                   but split into two exact divisions to keep values small.
                */
                seq_t tmp = C * (seq_t)(m - 1 - r);    /* divide by (r+1) next */
                tmp /= (seq_t)(r + 1);
                tmp *= (seq_t)(m - 1 - r - 1);          /* divide by (r+2) next */
                tmp /= (seq_t)(r + 2);
                C = tmp;
                r += 2;
            }

            /* next pow4 (divide exponent by 2 → divide by 16) */
            pow4 /= (seq_t)16;
        }

        /* ----------- compute S(cur) ----------- */
        seq_t S = 0;
        for (idx_t i = 2; i <= cur - 1; ++i) {
            seq_t prod = seq[i];
            prod *= seq[cur + 1 - i];
            S += prod;
        }

        /* ----------- a[cur] from recurrence ----------- */
        seq[cur] = (R - S) / (seq_t)2;
    }

    seq_len = n;
    return seq[n];
}
