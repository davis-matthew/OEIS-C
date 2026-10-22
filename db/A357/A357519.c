/*
 * A357519  Number of compositions (ordered partitions) of n into Jacobsthal
 *          numbers 1,3,5,11,21,43, ... (A001045).
 * https://oeis.org/A357519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ a(n – s) over all Jacobsthal numbers s ≤ n
 *            (Jacobsthal numbers: 1, 3, 5, 11, 21, …)
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#ifdef __cplusplus
#include <new>
#endif

/* --------------------------------------------------------------------------
   Small helper that allocates/deallocates a contiguous array of seq_t
   that works both in C (malloc/free) and C++ (new/delete[]).
   -------------------------------------------------------------------------- */
static seq_t *alloc_seq_arr(size_t cap)
{
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t *)malloc(cap * sizeof(seq_t));
#endif
}

static void free_seq_arr(seq_t *p)
{
#ifdef __cplusplus
    delete[] p;
#else
    free(p);
#endif
}

static void grow_seq_arr(seq_t **arr, size_t *cap, size_t need)
{
    if (need <= *cap) return;
    size_t new_cap = (*cap) ? (*cap) * 2 : 8;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = alloc_seq_arr(new_cap);
    for (size_t i = 0; i < *cap; ++i) tmp[i] = (*arr)[i];
    free_seq_arr(*arr);
    *arr = tmp;
    *cap = new_cap;
}

/* --------------------------------------------------------------------------
   Jacobsthal numbers handling
   -------------------------------------------------------------------------- */
static seq_t *jac_arr   = NULL;   /* array of distinct Jacobsthal numbers */
static size_t jac_cap   = 0;      /* allocated capacity of jac_arr    */
static idx_t   jac_len   = 0;      /* count of values in jac_arr       */
static seq_t   jac_last1 = 1;      /* J(k-1) of the last generated    */
static seq_t   jac_last2 = 0;      /* J(k-2) of the last generated    */

static void ensure_jac(idx_t n)
{
    /* initialize on first use */
    if (jac_cap == 0) {
        jac_cap = 8;
        jac_arr = alloc_seq_arr(jac_cap);
        jac_len = 0;
        if (n >= 1) {
            jac_arr[0] = (seq_t)1;
            jac_len   = 1;
        }
        /* jac_last1 = J(1) = 1, jac_last2 = J(0) = 0 */
    }

    /* generate additional Jacobsthal numbers until exceeding `n` */
    while (1) {
        seq_t next = jac_last1 + (seq_t)2 * jac_last2;
        if (next > (seq_t)n) break;
        if (next != jac_arr[jac_len - 1]) {     /* skip duplicate 1 */
            if (jac_len == jac_cap)
                grow_seq_arr(&jac_arr, &jac_cap, jac_cap + 1);
            jac_arr[jac_len++] = next;
        }
        jac_last2 = jac_last1;
        jac_last1 = next;
    }
}

/* --------------------------------------------------------------------------
   DP for the composition counts
   -------------------------------------------------------------------------- */
static seq_t *dp    = NULL;           /* memoised a(n) values */
static size_t dp_cap = 0;             /* allocated capacity of dp   */
static idx_t   dp_len = 0;            /* count of values pre‑computed */

static void ensure_dp(idx_t n)
{
    /* initialize dp array on first call */
    if (dp_len == 0) {
        dp_cap = 8;
        dp     = alloc_seq_arr(dp_cap);
        dp[0]  = (seq_t)1;            /* a(0) = 1 */
        dp_len = 1;
    }

    /* grow dp array if needed */
    if ((size_t)n >= dp_cap)
        grow_seq_arr(&dp, &dp_cap, (size_t)n + 1);

    /* compute values up to n */
    while (dp_len <= n) {
        idx_t i   = dp_len;                     /* current n to compute */
        ensure_jac(i);                          /* ensure parts ≤ i */
        seq_t sum = (seq_t)0;
        for (idx_t t = 0; t < jac_len; ++t) {
            seq_t s = jac_arr[t];
            if (s > (seq_t)i) break;
            sum = sum + dp[i - (idx_t)s];
        }
        dp[dp_len++] = sum;
    }
}

/* --------------------------------------------------------------------------
   Public function required by the challenge
   -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_dp(n);
    return dp[n];
}
