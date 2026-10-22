/*
 * A347841  a(n) is the number of (strict) chains of subspaces with ends 0 and
 *          (F_3)^n.
 * https://oeis.org/A347841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A347841: number of strict chains of subspaces in (F_3)^n with ends 0 and V_n
 *
 * Recurrence (q = 3):
 *    a(0) = 1
 *    a(n) = 1 + Σ_{d=1}^{n-1} C_3(n,d) * a(n-d)
 *    where C_3(n,d) is the Gaussian binomial coefficient over GF(3)
 *
 * The implementation memoises computed values so calls may come in any order.
 */

#include <stdlib.h>
#include <string.h>

/* cache for a(n) values */
static seq_t *a_cache = NULL;
static idx_t a_cache_capacity = 0;   /* number of allocated slots */
static idx_t a_max_computed = 0;     /* largest n for which a_cache[n] is valid */

/* cache for powers of 3: pow3[k] = 3^k */
static seq_t *pow3 = NULL;
static idx_t pow3_capacity = 0;      /* number of cached powers */

/* ensure the powers of 3 array can hold 3^n */
static void ensure_pow3(idx_t n)
{
    if (pow3_capacity > n)
        return;

    idx_t new_cap = n + 1;            /* we need indices 0..n inclusive */
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_arr == NULL)
        return;                       /* out of memory – caller will handle it */

    /* copy existing data if any */
    if (pow3 != NULL) {
        memcpy(new_arr, pow3, pow3_capacity * sizeof(seq_t));
        free(pow3);
    }

    /* if this is the first allocation set 3^0 = 1 */
    if (pow3_capacity == 0) {
        new_arr[0] = (seq_t)1;
        pow3_capacity = 1;
    }

    /* fill missing powers */
    for (idx_t i = pow3_capacity; i < new_cap; ++i) {
        new_arr[i] = new_arr[i - 1] * (seq_t)3;
    }
    pow3_capacity = new_cap;
    pow3 = new_arr;
}

/* ensure the a(n) cache can hold values up to n */
static void ensure_a_cache(idx_t n)
{
    if (a_cache_capacity > n)
        return;

    idx_t new_cap = n + 1;            /* indices 0..n inclusive */
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_arr == NULL)
        return;                       /* out of memory – caller will handle it */

    /* copy existing data if any */
    if (a_cache != NULL) {
        memcpy(new_arr, a_cache, a_cache_capacity * sizeof(seq_t));
        free(a_cache);
    }

    /* initialise base values */
    if (new_cap > 0)
        new_arr[0] = (seq_t)1;
    if (new_cap > 1)
        new_arr[1] = (seq_t)1;

    a_cache_capacity = new_cap;
    a_cache = new_arr;
}

/* compute a(n) for all n up to the requested value */
static void compute_a_up_to(idx_t n)
{
    /* make sure caches exist */
    ensure_a_cache(n);
    ensure_pow3(n);

    if (n <= a_max_computed)
        return;

    for (idx_t cur = a_max_computed + 1; cur <= n; ++cur) {
        seq_t sum = (seq_t)0;
        seq_t G_prev = (seq_t)1;      /* C_3(cur, 0) */

        for (idx_t d = 1; d < cur; ++d) {
            seq_t numerator = pow3[cur - d + 1] - (seq_t)1;
            seq_t denom     = pow3[d] - (seq_t)1;
            seq_t G_cur     = (G_prev * numerator) / denom; /* exact division */
            sum += G_cur * a_cache[cur - d];
            G_prev = G_cur;
        }
        a_cache[cur] = (seq_t)1 + sum;
    }
    if (n > a_max_computed)
        a_max_computed = n;
}

seq_t a(idx_t n)
{
    /* n is unsigned; no negative check needed */
    compute_a_up_to(n);
    return a_cache[n];
}
