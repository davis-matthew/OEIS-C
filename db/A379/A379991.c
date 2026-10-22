/*
 * A379991  Expansion of e.g.f. exp(-x)/(exp(-x) - x)^3.
 * https://oeis.org/A379991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n! * Sum_{k=0..n} (k+2)^(n-k) * binomial(k+2,2)/(n-k)!
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


/* Helper to grow an array of seq_t */
static void grow_seq_arr(seq_t **arr, idx_t *cap, idx_t need)
{
    if (*cap < need) {
        idx_t new_cap = *cap * 2;
        if (new_cap < need) new_cap = need;
        seq_t *tmp = NULL;
        if (*arr == NULL) {
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        } else {
            tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        }
        if (tmp == NULL) exit(1); /* allocation failure */
        *arr = tmp;
        *cap = new_cap;
    }
}

static seq_t *f_arr = NULL;    /* array of f_k values for current n   */
static idx_t  f_cap = 0;       /* capacity of f_arr                  */
static seq_t *a_arr = NULL;    /* cached a(k) values                 */
static idx_t  a_cap = 0;       /* capacity of a_arr                  */
static idx_t  cur_n = (idx_t)(-1); /* highest n computed so far        */
static seq_t  fact = 0;        /* factorial of current n             */

seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (cur_n == (idx_t)(-1)) {
        cur_n = 0;
        f_cap = 1;
        f_arr = (seq_t *)malloc(f_cap * sizeof(seq_t));
        if (f_arr == NULL) exit(1);
        f_arr[0] = (seq_t)1;         /* f_0(0) = 1 */

        fact = (seq_t)1;             /* 0! = 1 */

        a_cap = 1;
        a_arr = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (a_arr == NULL) exit(1);
        a_arr[0] = (seq_t)1;         /* a(0) = 1 */
    }

    /* Compute up to requested n */
    while (cur_n < n) {
        idx_t next = cur_n + 1;          /* new index to compute */

        /* Ensure enough space for f and a arrays */
        grow_seq_arr(&f_arr, &f_cap, next + 1);
        grow_seq_arr(&a_arr, &a_cap, next + 1);

        /* Update existing f_k values for k=0..cur_n using recurrence:
           f_k(next) = f_k(cur_n) * next * (k+2) / (next-k) */
        for (idx_t k = 0; k <= cur_n; ++k) {
            seq_t val = f_arr[k];
            val = val * (seq_t)next;                 /* multiply by n+1 */
            val = val * ((seq_t)(k + 2));            /* multiply by (k+2) */
            val = val / ((seq_t)(next - k));         /* exact division by (n+1-k) */
            f_arr[k] = val;
        }

        /* New factorial value for (next)! */
        seq_t newfact = fact * (seq_t)next;

        /* Compute binomial coefficient C(next+2,2) = (next+2)*(next+1)/2 */
        seq_t binom = ((seq_t)(next + 2) * (seq_t)(next + 1)) / (seq_t)2;

        /* New f_{next} value */
        f_arr[next] = newfact * binom;

        /* Sum all f_k to obtain a(next) */
        seq_t a_next = (seq_t)0;
        for (idx_t k = 0; k <= next; ++k) {
            a_next += f_arr[k];
        }
        a_arr[next] = a_next;

        /* Advance state */
        fact = newfact;
        cur_n = next;
    }

    return a_arr[n];
}
