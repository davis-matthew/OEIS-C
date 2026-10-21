/*
 * A249928  G.f. A(x) satisfies: 1+x = A(x)^2 + A(x)^5 - A(x)^6.
 * https://oeis.org/A249928
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. 1+x = A(x)^2 + A(x)^5 – A(x)^6. The sequence a(n) is the
 *            coefficient of x^n in the expansion of A(x) (with a(0)=1). Using
 *            series reversion we obtain the recurrence: a(0)=1, a(1)=1 For
 *            n≥2: let p2[n] = Σ_{i=1}^{n-1} a[i]·a[n-i] let p3[n] =
 *            Σ_{i=1}^{n-1} a[i]·p2[n-i] let p4[n] = Σ_{i=1}^{n-1}...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for the values and for intermediate powers */
static seq_t *a_seq           = NULL;
static seq_t *p2_seq          = NULL;
static seq_t *p3_seq          = NULL;
static seq_t *p4_seq          = NULL;
static seq_t *p5_seq          = NULL;
static seq_t *p6_seq          = NULL;
static idx_t  max_cached_n    = 0;   /* largest n for which a_seq is filled */

/* helper to resize the cached arrays up to at least size n+1 */
static void
ensure_capacity(idx_t n)
{
    size_t needed = (size_t)n + 1;          /* indices 0 .. n */

    if (a_seq == NULL) {                   /* first allocation */
        a_seq  = (seq_t*)malloc(needed * sizeof(seq_t));
        p2_seq = (seq_t*)malloc(needed * sizeof(seq_t));
        p3_seq = (seq_t*)malloc(needed * sizeof(seq_t));
        p4_seq = (seq_t*)malloc(needed * sizeof(seq_t));
        p5_seq = (seq_t*)malloc(needed * sizeof(seq_t));
        p6_seq = (seq_t*)malloc(needed * sizeof(seq_t));

        for (size_t i = 0; i < needed; ++i) {
            a_seq[i] = 0;
            p2_seq[i] = 0;
            p3_seq[i] = 0;
            p4_seq[i] = 0;
            p5_seq[i] = 0;
            p6_seq[i] = 0;
        }
    } else if (needed > max_cached_n + 1) { /* grow existing arrays */
        a_seq  = (seq_t*)realloc(a_seq,  needed * sizeof(seq_t));
        p2_seq = (seq_t*)realloc(p2_seq, needed * sizeof(seq_t));
        p3_seq = (seq_t*)realloc(p3_seq, needed * sizeof(seq_t));
        p4_seq = (seq_t*)realloc(p4_seq, needed * sizeof(seq_t));
        p5_seq = (seq_t*)realloc(p5_seq, needed * sizeof(seq_t));
        p6_seq = (seq_t*)realloc(p6_seq, needed * sizeof(seq_t));

        for (size_t i = max_cached_n + 1; i < needed; ++i) {
            a_seq[i] = 0;
            p2_seq[i] = 0;
            p3_seq[i] = 0;
            p4_seq[i] = 0;
            p5_seq[i] = 0;
            p6_seq[i] = 0;
        }
    }

    a_seq[0] = 1;          /* constant term of A(x) */

    /* compute terms from the next needed index up to n */
    for (size_t k = max_cached_n + 1; k <= (size_t)n; ++k) {
        if (k == 1) {          /* base case a(1)=1 */
            a_seq[1] = 1;
            continue;
        }

        seq_t p2 = 0, p3 = 0, p4 = 0, p5 = 0, p6 = 0;

        for (size_t i = 1; i < k; ++i) {
            p2 += a_seq[i] * a_seq[k - i];
        }
        p2_seq[k] = p2;

        for (size_t i = 1; i < k; ++i) {
            p3 += a_seq[i] * p2_seq[k - i];
        }
        p3_seq[k] = p3;

        for (size_t i = 1; i < k; ++i) {
            p4 += a_seq[i] * p3_seq[k - i];
        }
        p4_seq[k] = p4;

        for (size_t i = 1; i < k; ++i) {
            p5 += a_seq[i] * p4_seq[k - i];
        }
        p5_seq[k] = p5;

        for (size_t i = 1; i < k; ++i) {
            p6 += a_seq[i] * p5_seq[k - i];
        }
        p6_seq[k] = p6;

        a_seq[k] = 4 * p2 + 10 * p3 + 10 * p4 + 5 * p5 + p6;
    }

    max_cached_n = (idx_t)n;
}

seq_t
a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    if ((size_t)n > max_cached_n) {
        ensure_capacity(n);
    }

    return a_seq[(size_t)n];
}
