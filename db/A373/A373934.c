/*
 * A373934  Number of compositions of 7*n-2 into parts 6 and 7.
 * https://oeis.org/A373934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of compositions of 7*n-2 into parts 6 and 7.
   This implementation uses a simple dynamic‑programming recurrence:
   f(m) = f(m‑6) + f(m‑7) with f(0) = 1 (empty composition) and
   f(m) = 0 for 0 < m < 6.  The desired term is a(n) = f(7*n‑2).  */
/*  The sequence can be computed on demand and cached for fast
   repeated calls.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

#ifdef __cplusplus
#define OEISC_VPP(p) (p)
#define OEISC_VP(p) (p)
#else
#define OEISC_VPP(p) ((void **)(p))
#define OEISC_VP(p) ((void *)(p))
#endif

/* Grow a dynamic array when needed.  The pointer is passed as OEISC_VPP(.) */
#ifdef __cplusplus
template <class T>
static void ensure_capacity(size_t needed, size_t *cap, T **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    T *tmp = mem_resize(T, *ptr, (new_cap * elem_size) / sizeof(T));
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#else
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}
#endif

/* External linkage as required by the framework. */
seq_t a(idx_t n)
{
    /* a(1) = 0, because 7*1-2 = 5 cannot be formed from 6 and 7. */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Target sum m = 7*n - 2.  */
    idx_t m = 7 * n - 2;

    /* Static cache for the DP array.  We keep the maximum index
       currently computed in dp_len.  */
    static seq_t *dp      = NULL;     /* DP array of f(i) values.  */
    static size_t dp_cap = 0;         /* Current capacity of dp. */
    static idx_t  dp_len = 0;         /* Next index that is not yet valid. */

    /* Ensure the DP array is large enough to hold indices up to m.  */
    ensure_capacity(m + 1, &dp_cap, OEISC_VPP(&dp), sizeof(seq_t));

    /* Initialise the base value f(0) = 1 if this is the first run.  */
    if (dp_len == 0) {
        dp[0] = (seq_t)1;
        dp_len = 1;
    }

    /* Compute all missing values up to index m.  */
    for (idx_t i = dp_len; i <= m; ++i) {
        seq_t val6 = (i >= 6) ? dp[i - 6] : (seq_t)0;
        seq_t val7 = (i >= 7) ? dp[i - 7] : (seq_t)0;
        dp[i] = val6 + val7;
    }

    /* Update the computed length.  */
    dp_len = m + 1;

    /* The required term.  */
    return dp[m];
}
