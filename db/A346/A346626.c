/*
 * A346626  G.f. A(x) satisfies: A(x) = (1 + x * A(x)^3) / (1 - x).
 * https://oeis.org/A346626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + Σ_{i=0}^{n-1} Σ_{j=0}^{n-i-1}
 *            a(i)·a(j)·a(n-i-j-1), with a(0) = 1. This is equivalent to the
 *            generating‑function equation A(x) = (1 + x·A(x)^3)/(1-x). The
 *            implementation keeps the sequence A in an array and the
 *            coefficients of A^2 in a second array, updating both in O(n^2)
 *            time wh...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

/* Multiply two seq_t values, using a wider intermediate type when
   seq_t is a small built‑in integer to avoid overflow.  For larger
   seq_t types (e.g. big integer classes) the ordinary operator*
   is used. */
static inline seq_t mul_seq_t(seq_t x, seq_t y)
{
    /* The following cast is safe for small seq_t; for larger types
       the condition is false and ordinary multiplication is used. */
    if (sizeof(seq_t) <= sizeof(unsigned long long))
        return (seq_t)((unsigned long long)x * (unsigned long long)y);
    return x * y;
}

/* Ensure that an array has room for at least `need+1` elements.
   Newly allocated slots are zeroed. */
static void ensure_cap(seq_t **arr, size_t *cap, size_t need)
{
    if (*cap <= need) {
        size_t new_cap = (*cap == 0 ? 1 : *cap);
        while (new_cap <= need)
            new_cap <<= 1;
        *arr = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        for (size_t i = *cap; i < new_cap; ++i)
            (*arr)[i] = 0;
        *cap = new_cap;
    }
}

/* Static state for memoisation */
static seq_t *seq_vals = NULL;   /* a[0..] */
static seq_t *sq_vals  = NULL;   /* D[m] = Σ_{i=0}^m a[i] * a[m-i] */
static size_t vals_cap = 0, sq_cap = 0;
static idx_t last_computed = (idx_t)-1;

/* Public function */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;

    /* Initialise on first call */
    if (last_computed == (idx_t)-1) {
        ensure_cap(&seq_vals, &vals_cap, 0);
        seq_vals[0] = (seq_t)1;
        last_computed = 0;

        ensure_cap(&sq_vals, &sq_cap, 0);
        sq_vals[0] = mul_seq_t(seq_vals[0], seq_vals[0]);  /* 1 */
    }

    if (N <= (size_t)last_computed)  /* cached value */
        return seq_vals[N];

    ensure_cap(&seq_vals, &vals_cap, N);
    ensure_cap(&sq_vals,  &sq_cap,   2 * N);

    /* Compute values up to N */
    for (size_t k = (size_t)last_computed + 1; k <= N; ++k) {
        /* Coefficient of x^{k-1} in A^3 = (D * a)[k-1] */
        seq_t B = 0;
        size_t m = k - 1;
        for (size_t i = 0; i < k; ++i) {
            seq_t prod = mul_seq_t(seq_vals[i], sq_vals[m - i]);
            B += prod;
        }
        seq_vals[k] = seq_vals[k - 1] + B;

        /* Update sq_vals with the new a[k] */
        seq_t ak = seq_vals[k];
        for (size_t i = 0; i < k; ++i) {
            seq_t prod = mul_seq_t(seq_vals[i], ak);
            sq_vals[k + i] += prod + prod;  /* both ordered pairs */
        }
        sq_vals[2 * k] += mul_seq_t(ak, ak);
    }

    last_computed = N;
    return seq_vals[N];
}
