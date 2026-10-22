/*
 * A323186  a(0) = 0, a'(0) = 0, a''(0) = 1, a''(1) = -1, a(n) = a(n-1) +
 *          a'(n), a'(n) = a'(n-1) + a''(n), a''(n) = -a''(n-1) if a(n-2) = 0,
 *          or else a''(n-1).
 * https://oeis.org/A323186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, look
 * Formula:   not stated
 * Range:     i8 n <= 267, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A323186
 * Recurrence:
 *   a(0) = 0, a'(0) = 0, a''(0) = 1
 *   a''(1) = -1
 *   a''(n) = -a''(n-1)  if a(n-2) == 0
 *           =  a''(n-1)  otherwise
 *   a'(n) = a'(n-1) + a''(n)
 *   a(n)  = a(n-1) + a'(n)
 * (indices start at 0)
 */

#include <stdlib.h>
/* if compiled as C++ the explicit casts of void* to seq_t* are required   */

static seq_t *a_arr    = NULL;   /* a(n)  */
static seq_t *ap_arr   = NULL;   /* a'(n) */
static seq_t *app_arr  = NULL;   /* a''(n) */
static size_t cap      = 0;      /* allocated size for arrays */
static idx_t  count    = 0;      /* number of elements stored (0 means none) */

/* enlarge the arrays so that indices 0..n are valid */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cap) return;

    size_t new_cap = (size_t)(n + 1) * 2;

    /* reallocate a_arr */
    if (a_arr == NULL) {
        a_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    } else {
        a_arr = (seq_t *)realloc(a_arr, new_cap * sizeof(seq_t));
    }

    /* reallocate ap_arr */
    if (ap_arr == NULL) {
        ap_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    } else {
        ap_arr = (seq_t *)realloc(ap_arr, new_cap * sizeof(seq_t));
    }

    /* reallocate app_arr */
    if (app_arr == NULL) {
        app_arr = (seq_t *)calloc(new_cap, sizeof(seq_t));
    } else {
        app_arr = (seq_t *)realloc(app_arr, new_cap * sizeof(seq_t));
    }

    cap = new_cap;
}

/* compute all values up to and including index n */
static void compute_upto(idx_t n)
{
    if (count > n) return;         /* already computed */

    ensure_capacity(n);

    for (idx_t i = count; i <= n; ++i) {
        if (i == 0) {
            a_arr[i]    = (seq_t)0;
            ap_arr[i]   = (seq_t)0;
            app_arr[i]  = (seq_t)1;
        } else if (i == 1) {
            app_arr[i] = -(app_arr[0]);                  /* a''(1) = -1 */
            ap_arr[i]  = ap_arr[0] + app_arr[i];          /* a'(1)  = -1 */
            a_arr[i]   = a_arr[0] + ap_arr[i];           /* a(1)   = -1 */
        } else {
            /* a''(i) depends on a(i-2) */
            if (a_arr[i - 2] == (seq_t)0) {
                app_arr[i] = -(app_arr[i - 1]);           /* flip sign */
            } else {
                app_arr[i] = app_arr[i - 1];              /* keep sign */
            }
            ap_arr[i]  = ap_arr[i - 1] + app_arr[i];      /* a'(i)  = a'(i-1)+a''(i) */
            a_arr[i]   = a_arr[i - 1] + ap_arr[i];       /* a(i)   = a(i-1)+a'(i) */
        }
    }
    count = n + 1;   /* we now have indices 0..n computed */
}

/* external function required by the build */
seq_t a(idx_t n)
{
    compute_upto(n);
    return a_arr[n];
}
