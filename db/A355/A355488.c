/*
 * A355488  Expansion of g.f. f/(1+2*f) where f is the g.f. of nonempty
 *          permutations.
 * https://oeis.org/A355488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! - 2 · Σ_{k=1..n} k! · a(n‑k) (a(0)=0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A355488 – Expansion of f/(1+2f) where f is the G.f. of non‑empty permutations.
    The recurrence follows from (1+2f)·a = f.  */
/*  The implementation keeps factorials and previously computed a(n) in
    dynamic arrays, extending them only when a larger index is requested.  */

#include <stdlib.h>

/* local helpers – not part of the forbidden name list */
static void grow_array(seq_t **arr, size_t *size, size_t new_cap)
{
    seq_t *new_arr = NULL;
#ifdef __cplusplus
    new_arr = new seq_t[new_cap];
    for (size_t i = 0; i < *size && i < new_cap; ++i)
        new_arr[i] = (*arr)[i];
    delete[] *arr;
#else
    new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    for (size_t i = 0; i < *size && i < new_cap; ++i)
        new_arr[i] = (*arr)[i];
    free(*arr);
#endif
    *arr = new_arr;
    *size = new_cap;
}

/* factorial cache ----------------------------------------------------- */
static size_t fact_size = 0;
static seq_t *fact_arr  = NULL;

static void ensure_factorials(idx_t n)
{
    /* need factorials up to n (index 0..n) */
    size_t target = (size_t)n + 1;
    if (target > fact_size) {
        size_t old_size = fact_size;
        size_t new_cap  = target;

        if (fact_size == 0) {   /* first allocation */
#ifdef __cplusplus
            fact_arr = new seq_t[new_cap];
#else
            fact_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
            fact_arr[0] = (seq_t)1;
            fact_size  = 1;
            old_size   = 1;     /* 0! already present */
        } else {
            grow_array(&fact_arr, &fact_size, new_cap);
        }

        /* compute missing factorial entries */
        for (size_t i = old_size; i < target; ++i)
            fact_arr[i] = fact_arr[i-1] * (seq_t)i;

        fact_size = target;
    }
}

/* a(n) cache -------------------------------------------------------- */
static size_t a_cap = 0;
static seq_t *a_arr  = NULL;

static void ensure_a(idx_t n)
{
    size_t target = (size_t)n + 1;
    if (target > a_cap) {
        size_t old_cap = a_cap;
        size_t new_cap = target;

        if (old_cap == 0) {
#ifdef __cplusplus
            a_arr = new seq_t[new_cap];
#else
            a_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif
            a_arr[0] = (seq_t)0;    /* a(0) = 0 */
            a_cap = 1;
            old_cap = 1;
        } else {
            grow_array(&a_arr, &a_cap, new_cap);
        }

        for (size_t i = old_cap; i < target; ++i) {      /* i >= 1 */
            ensure_factorials((idx_t)i);

            seq_t sum = (seq_t)0;
            for (size_t k = 1; k <= i; ++k)
                sum = sum + fact_arr[k] * a_arr[i - k];

            a_arr[i] = fact_arr[i] - sum * (seq_t)2;
        }
        a_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    ensure_a(n);
    return a_arr[(size_t)n];
}
