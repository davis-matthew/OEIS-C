/*
 * A294537  Solution of the complementary equation a(n) = a(n-1) + a(n-2) +
 *          b(n-2) + n, where a(0) = 1, a(1) = 2, b(0) = 3.
 * https://oeis.org/A294537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + b(n-2) + n; Init: a(0)=1, a(1)=2,
 *            b(0)=3 The sequences a() and b() are complementary increasing
 *            sequences, constructed by taking the smallest unused integer for
 *            each b(k)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* helper: binary search to test whether a value is present
   in the currently built portions of a[] and b[]. */
static bool value_used(seq_t val,
                       const seq_t *a, idx_t a_len,
                       const seq_t *b, idx_t b_len)
{
    /* search in a[0..a_len-1] */
    idx_t lo = 0, hi = a_len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (a[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < a_len && a[lo] == val)
        return true;

    /* search in b[0..b_len-1] */
    lo = 0; hi = b_len;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (b[mid] < val)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < b_len && b[lo] == val)
        return true;

    return false;
}

seq_t a(idx_t n)
{
    /* static storage for computed a[] and b[] */
    static seq_t *a_arr = NULL;
    static seq_t *b_arr = NULL;
    static size_t a_cap = 0;
    static size_t b_cap = 0;
    static idx_t a_len = 0;
    static idx_t b_len = 0;

    /* ensure base values are present */
    if (a_len == 0) {
        size_t cap = 2;
        if (n + 1 > cap) cap = n + 1;
        a_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        a_cap = cap;
        a_arr[0] = (seq_t)1;
        a_len = 1;
    }
    if (a_len == 1) {
        if (a_len >= (idx_t)a_cap) {
            a_cap *= 2;
            a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
        }
        a_arr[1] = (seq_t)2;
        a_len = 2;
    }
    if (b_len == 0) {
        size_t cap = 1;
        if (n + 1 > cap) cap = n + 1;
        b_arr = (seq_t *)malloc(cap * sizeof(seq_t));
        b_cap = cap;
        b_arr[0] = (seq_t)3;
        b_len = 1;
    }

    /* fast answer if n already computed */
    if (n < a_len)
        return a_arr[n];

    /* grow arrays if necessary */
    size_t need = (size_t)(n + 1);
    if (need > a_cap) {
        a_cap = need;
        a_arr = (seq_t *)realloc(a_arr, a_cap * sizeof(seq_t));
    }
    if (need > b_cap) {
        b_cap = need;
        b_arr = (seq_t *)realloc(b_arr, b_cap * sizeof(seq_t));
    }

    /* compute terms sequentially up to n */
    for (idx_t i = a_len; i <= n; ++i) {
        /* compute a[i] */
        seq_t ai;
        if (i == 0)
            ai = (seq_t)1;
        else if (i == 1)
            ai = (seq_t)2;
        else
            ai = a_arr[i - 1] + a_arr[i - 2] + b_arr[i - 2] + (seq_t)i;
        a_arr[i] = ai;
        a_len = i + 1;

        /* compute b[i-1] (if i>=2) */
        if (i >= 2) {
            idx_t j = i - 1;                 /* index for new b element */
            seq_t cand = b_arr[j - 1] + (seq_t)1;  /* start from previous b + 1 */
            while (value_used(cand, a_arr, a_len, b_arr, j))
                cand = cand + (seq_t)1;
            b_arr[j] = cand;
            b_len = j + 1;
        }
    }

    return a_arr[n];
}
