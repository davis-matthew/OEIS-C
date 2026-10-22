/*
 * A398001  E.g.f. A(x) satisfies A'(x) = 1 + A((exp(2*x) - 1)/2), with A(0) =
 *          0.
 * https://oeis.org/A398001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} 2^(n-1-k) * Stirling2(n-1,k) * a(k) with
 *            a(1) = 1 (offset 1). The recurrence is implemented using the
 *            dynamic array of values a[1..n] and the auxiliary array w[k] =
 *            2^(n-k) * Stirling2(n,k). For each increment n → n+1 this
 *            auxiliary array satisfies the update rule: w_k(n+1)...
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *a_arr = NULL;      /* 0‑unused, a_arr[1] = a(1) ...      */
static seq_t *w_arr = NULL;      /* 0‑unused, w_arr[1] = 1 at start   */
static size_t a_size = 0;        /* allocated length of a_arr          */
static size_t w_size = 0;        /* allocated length of w_arr          */
static idx_t  max_computed = 0;  /* highest n for which a_arr[n] known */

/* Grow an array to hold at least `newlen` elements, initializing
   any newly allocated part to zero.  The array must be a pointer to
   a seq_t pointer. */
static void grow_array(seq_t **arr, size_t *len, size_t newlen)
{
    if (*len >= newlen) return;
    seq_t *tmp = (seq_t *)realloc(*arr, newlen * sizeof(seq_t));
    if (!tmp) abort();                      /* out of memory */
    /* zero the newly allocated part */
    for (size_t i = *len; i < newlen; ++i) tmp[i] = 0;
    *arr = tmp;
    *len = newlen;
}

/* Ensure that a_arr and w_arr contain values up to index `n`.  */
static void ensure(idx_t n)
{
    if (n <= max_computed) return;

    /* Allocate or grow the arrays to hold at least n+1 elements
       (index 0 is unused). */
    grow_array(&a_arr, &a_size, (size_t)n + 1);
    grow_array(&w_arr, &w_size, (size_t)n + 1);

    /* Base case for the sequence (a(1)=1).  If max_computed==0
       the arrays are freshly allocated and uninitialised. */
    if (max_computed == 0) {
        a_arr[1] = 1;
        w_arr[1] = 1;   /* 2^(1-1)*S(1,1) = 1 */
        max_computed = 1;
    }

    /* Grow the sequence up to index n. */
    for (idx_t cur = max_computed; cur < n; ++cur) {
        /* Compute a(cur+1) = Σ_{k=1..cur} w_k(cur) * a_k. */
        seq_t sum = 0;
        for (idx_t k = 1; k <= cur; ++k) {
            sum += w_arr[k] * a_arr[k];
        }
        a_arr[cur + 1] = sum;

        /* Update w array to represent w_k(cur+1) for the next n. */
        /* First handle k ≥ 2; use a descending loop to avoid overwrite. */
        for (idx_t k = cur; k > 1; --k) {
            w_arr[k] = ((seq_t)(2 * k)) * w_arr[k] + w_arr[k - 1];
        }
        /* Handle k = 1 separately. */
        w_arr[1] = ((seq_t)2) * w_arr[1];

        /* The new entry w_cur+1(cur+1) = 1, as S(cur+1,cur+1)=1. */
        w_arr[cur + 1] = 1;
    }

    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for offset 1 */
    ensure(n);
    return a_arr[n];
}
