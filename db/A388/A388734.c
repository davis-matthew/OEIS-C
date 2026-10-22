/*
 * A388734  G.f. satisfies A(x) = 1 + x*A(x)^2 + x^2*(1-x)*A(x)^3.
 * https://oeis.org/A388734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 30, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A388734
   G.f. satisfies  A(x) = 1 + x*A(x)^2 + x^2*(1‑x)*A(x)^3.
   Recurrence:   a(0)=1,
                  for n≥1
                     a(n) = d(n‑1) + (c(n‑2) – c(n‑3)),
                     d(k)=∑_{i=0}^{k} a(i)*a(k‑i)     (coefficients of A^2),
                     c(k)=∑_{i=0}^{k} a(i)*d(k‑i)     (coefficients of A^3).  */

#include <stdlib.h>
#include <stddef.h>

static idx_t max_computed = 0;      /* largest n for which arrays are ready   */
static seq_t *a_arr = NULL;         /* a(n)                               */
static seq_t *d_arr = NULL;         /* coefficients of A^2                */
static seq_t *c_arr = NULL;         /* coefficients of A^3                */
static size_t cap_a = 0, cap_d = 0, cap_c = 0;  /* current capacities                     */

/* Allocate and initialise the three arrays on first use.           */
static void initialise_arrays(void)
{
    if (a_arr != NULL)
        return;                                 /* already done */

    /* start with capacity 1 for each array */
    size_t cap = 1;
    a_arr = (seq_t *)malloc(cap * sizeof(seq_t));
    d_arr = (seq_t *)malloc(cap * sizeof(seq_t));
    c_arr = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!a_arr || !d_arr || !c_arr)
        exit(1);                                   /* memory allocation failure */

    /* initial values  a(0)=1, d(0)=a0*a0=1, c(0)=a0*a0*a0=1   */
    a_arr[0] = (seq_t)1;
    d_arr[0] = (seq_t)1;
    c_arr[0] = (seq_t)1;

    cap_a = cap_d = cap_c = 1;
    max_computed = 0;
}

/* Resize a pointer array if it needs at least 'needed' elements.   */
static void ensure_capacity(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed)
        return;

    size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
    while (new_cap < needed)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(1);                                   /* memory allocation failure */

    *arr = tmp;
    *cap = new_cap;
}

/* Computes a(k), d(k) and c(k) for all k≤n and stores them.        */
static void compute_until(idx_t n)
{
    initialise_arrays();           /* ensure a_arr, d_arr, c_arr exist */

    if (n <= max_computed)
        return;                     /* nothing to do */

    /* iterate from next index after the last computed one */
    for (idx_t k = max_computed + 1; k <= n; ++k)
    {
        /* ---------- a(k) ------------------------------------ */
        seq_t term = d_arr[k - 1];            /* d(k-1)                                      */
        if (k >= 2)
            term += c_arr[k - 2];             /* + c(k-2)                                    */
        if (k >= 3)
            term -= c_arr[k - 3];             /* - c(k-3)                                    */

        ensure_capacity(&a_arr, &cap_a, k + 1);
        a_arr[k] = term;

        /* ---------- d(k) ------------------------------------ */
        seq_t sum_d = 0;
        for (idx_t i = 0; i <= k; ++i)
            sum_d += a_arr[i] * a_arr[k - i];

        ensure_capacity(&d_arr, &cap_d, k + 1);
        d_arr[k] = sum_d;

        /* ---------- c(k) ------------------------------------ */
        seq_t sum_c = 0;
        for (idx_t i = 0; i <= k; ++i)
            sum_c += a_arr[i] * d_arr[k - i];

        ensure_capacity(&c_arr, &cap_c, k + 1);
        c_arr[k] = sum_c;
    }

    max_computed = n;            /* new range is ready */
}

seq_t a(idx_t n)
{
    compute_until(n);
    return a_arr[n];
}
