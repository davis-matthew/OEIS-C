/*
 * A394143  G.f. A(x) satisfies A(x)^4-3*A(x)^3+(3-x)*A(x)^2-A(x)-x^2+x=0.
 * https://oeis.org/A394143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A394143
    The recurrence was derived from the functional equation
    A(x)^4 - 3*A(x)^3 + (3 - x)*A(x)^2 - A(x) - x^2 + x = 0.
    For n ≥ 0:
       a[n] = c4[n] - 3*c3[n] + 3*c2[n] - c2[n-1] + δ(n,1) - δ(n,2)
    where
       c2[n] = Σ_{i=0}^{n} a[i] * a[n-i]
       c3[n] = Σ_{i=0}^{n} a[i] * c2[n-i]
       c4[n] = Σ_{i=0}^{n} a[i] * c3[n-i]
    with a[0] = 0, a[1] = 1.  The arrays are built incrementally and
    memoised in static storage so successive calls are fast.
*/

#ifdef __cplusplus
#include <vector>
#endif
#include <stddef.h>   /* for size_t */

/* static storage for memoisation */
#ifdef __cplusplus
static std::vector<seq_t> a_vals;
static std::vector<seq_t> c2_vals;
static std::vector<seq_t> c3_vals;
static std::vector<seq_t> c4_vals;
#else
static seq_t *a_vals = NULL;
static seq_t *c2_vals = NULL;
static seq_t *c3_vals = NULL;
static seq_t *c4_vals = NULL;
static size_t a_len = 0;   /* current number of stored elements */
#endif

/* --------------------------------------------------------------------
   Ensure that the sequence arrays hold values up to index n (inclusive).
   -------------------------------------------------------------------- */
static void ensure_max(idx_t n)
{
    size_t target = (size_t)n + 1;

#ifdef __cplusplus
    /* C++ implementation: use std::vector */
    size_t old = a_vals.size();
    if (old < target) {
        /* enlarge all arrays */
        a_vals.resize(target);
        c2_vals.resize(target);
        c3_vals.resize(target);
        c4_vals.resize(target);

        /* initialise base values (overwritten if already present) */
        a_vals[0] = 0;
        if (target > 1) a_vals[1] = 1;
        c2_vals[0] = 0;
        c3_vals[0] = 0;
        c4_vals[0] = 0;

        if (old < 2) old = 2;   /* first index to compute */

        for (size_t k = old; k < target; ++k) {
            /* compute c2[k] */
            seq_t sum2 = 0;
            for (size_t i = 0; i <= k; ++i)
                sum2 += a_vals[i] * a_vals[k - i];
            c2_vals[k] = sum2;

            /* compute c3[k] */
            seq_t sum3 = 0;
            for (size_t i = 0; i <= k; ++i)
                sum3 += a_vals[i] * c2_vals[k - i];
            c3_vals[k] = sum3;

            /* compute c4[k] */
            seq_t sum4 = 0;
            for (size_t i = 0; i <= k; ++i)
                sum4 += a_vals[i] * c3_vals[k - i];
            c4_vals[k] = sum4;

            /* recurrence for a[k] */
            seq_t val = c4_vals[k] - 3 * c3_vals[k] + 3 * c2_vals[k];
            val -= c2_vals[k - 1];
            if (k == 1) val += 1;
            if (k == 2) val -= 1;
            a_vals[k] = val;
        }
    }
#else
    /* C implementation: use raw realloc */
    if (a_len < target) {
        a_vals  = (seq_t*)realloc(a_vals,  target * sizeof(seq_t));
        c2_vals = (seq_t*)realloc(c2_vals, target * sizeof(seq_t));
        c3_vals = (seq_t*)realloc(c3_vals, target * sizeof(seq_t));
        c4_vals = (seq_t*)realloc(c4_vals, target * sizeof(seq_t));

        /* initialise base values */
        a_vals[0] = 0;
        c2_vals[0] = 0;
        c3_vals[0] = 0;
        c4_vals[0] = 0;
        if (target > 1) {
            a_vals[1] = 1;
        }

        size_t start = (a_len < 2) ? 2 : a_len;   /* first index to compute */

        for (size_t k = start; k < target; ++k) {
            /* compute c2[k] */
            seq_t sum2 = 0;
            for (size_t i = 0; i <= k; ++i)
                sum2 += a_vals[i] * a_vals[k - i];
            c2_vals[k] = sum2;

            /* compute c3[k] */
            seq_t sum3 = 0;
            for (size_t i = 0; i <= k; ++i)
                sum3 += a_vals[i] * c2_vals[k - i];
            c3_vals[k] = sum3;

            /* compute c4[k] */
            seq_t sum4 = 0;
            for (size_t i = 0; i <= k; ++i)
                sum4 += a_vals[i] * c3_vals[k - i];
            c4_vals[k] = sum4;

            /* recurrence for a[k] */
            seq_t val = c4_vals[k] - 3 * c3_vals[k] + 3 * c2_vals[k];
            val -= c2_vals[k - 1];
            if (k == 1) val += 1;
            if (k == 2) val -= 1;
            a_vals[k] = val;
        }
        a_len = target;
    }
#endif
}

/* --------------------------------------------------------------------
   Public function required by the build script.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_max(n);
    return a_vals[(size_t)n];
}
