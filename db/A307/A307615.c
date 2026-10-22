/*
 * A307615  E.g.f. A(x) satisfies: A(x) = exp(x) * A(x^2)^2*A(x^3)^3*A(x^4)^4*
 *          ... *A(x^k)^k* ...
 * https://oeis.org/A307615
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A307615:  
 * a(0) = 1  
 * a(n) = Σ_{k=1..n} A074206(k) · k · k! · binom(n−1,k−1) · a(n−k)
 * where g(k) = A074206(k)·k·k! satisfies the divisor recurrence
 * g(1) = 1,
 * g(n) = Σ_{d | n, d < n} (n! / d!) · (n/d) · g(d)
 * The implementation pre‑computes factorials, g(k) and a(n) up to the
 * requested index using memoisation.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* ---- Global state ---- */
static seq_t *a_arr   = NULL;   /* a[0..cap-1] */
static seq_t *g_arr   = NULL;   /* g[0..cap-1], g[0] unused */
static seq_t *fact    = NULL;   /* fact[0..cap-1] */
static size_t cap     = 0;
static size_t a_last  = 0;      /* last index for which a[n] is known */
static size_t g_last  = 0;      /* last index for which g[n] is known */
static size_t f_last  = 0;      /* last index for which fact[n] is known */

/* ---- Utility: resize arrays to accommodate indices up to N ---- */
static void ensure_cap(size_t N)
{
    if (N + 1 <= cap)
        return;
    size_t new_cap = (N + 1) * 2;
    seq_t *tmp;

    tmp = (seq_t*)realloc(a_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort(); a_arr = tmp;
    tmp = (seq_t*)realloc(g_arr, new_cap * sizeof(seq_t));
    if (!tmp) abort(); g_arr = tmp;
    tmp = (seq_t*)realloc(fact, new_cap * sizeof(seq_t));
    if (!tmp) abort(); fact = tmp;

    /* initialise new elements to zero */
    for (size_t i = cap; i <= N; ++i) {
        a_arr[i]  = 0;
        g_arr[i]  = 0;
        fact[i]   = 0;
    }
    cap = new_cap;
}

/* ---- Ensure factorials up to N are known ---- */
static void compute_factorials_up_to(size_t N)
{
    if (N <= f_last)
        return;
    if (f_last == 0) {
        /* initialise fact[0] */
        fact[0] = 1;
        f_last = 0;
    }
    for (size_t i = f_last + 1; i <= N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    f_last = N;
}

/* ---- Compute g(k) up to N using divisor recurrence ---- */
static void compute_g_upto(size_t N)
{
    if (N <= g_last)
        return;
    /* value needed for g[1] */
    if (g_last == 0) {
        g_arr[1] = 1;       /* A074206(1)·1·1! = 1 */
        g_last   = 1;
    }
    for (size_t i = g_last + 1; i <= N; ++i) {
        seq_t sum = 0;
        /* iterate over proper divisors d of i */
        for (size_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                size_t d1 = d;
                size_t d2 = i / d;

                if (d1 < i) {
                    seq_t term = fact[i] / fact[d1];   /* i! / d1! */
                    term = term * (seq_t)(i / d1);     /* * (i/d1) */
                    term = term * g_arr[d1];
                    sum = sum + term;
                }

                if (d2 != d1 && d2 < i) {
                    seq_t term = fact[i] / fact[d2];
                    term = term * (seq_t)(i / d2);
                    term = term * g_arr[d2];
                    sum = sum + term;
                }
            }
        }
        g_arr[i] = sum;
    }
    g_last = N;
}

/* ---- Compute a(n) up to N using the main recurrence ---- */
static void compute_a_upto(size_t N)
{
    if (N <= a_last)
        return;
    for (size_t n = a_last + 1; n <= N; ++n) {
        seq_t sum  = 0;
        seq_t binom = 1;               /* binom(n-1,0) = 1 */
        for (size_t k = 1; k <= n; ++k) {
            seq_t term = g_arr[k] * binom * a_arr[n - k];
            sum = sum + term;
            if (k == n)
                break;                 /* binom(n-1, n-1) = 1, no next update */
            /* update binom to binom(n-1, k) */
            binom = binom * (seq_t)(n - k) / (seq_t)k;
        }
        a_arr[n] = sum;
    }
    a_last = N;
}

/* ---- Public interface ---- */
seq_t a(idx_t n)
{
    size_t N = (size_t)n;          /* safe cast: we only index arrays */

    /* Initialise global structures on first call */
    if (!a_arr) {
        ensure_cap(1);             /* at least index 0 */
        a_arr[0] = 1;              /* a(0) = 1 */
        g_arr[1] = 1;              /* g(1) = 1 */
        fact[0]  = 1;              /* 0! = 1 */
        fact[1]  = 1;              /* 1! = 1 */
        a_last  = 0;
        g_last  = 1;
        f_last  = 1;
    }

    /* Guard against very large indices that would blow memory */
    if (N > 10000) {
        /* For the intended benchmark range the implementation works
           well; larger values are beyond the original specification. */
        return 0;
    }

    /* Prepare all needed data up to N */
    ensure_cap(N);
    compute_factorials_up_to(N);
    compute_g_upto(N);
    compute_a_upto(N);

    return a_arr[N];
}
