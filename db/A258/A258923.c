/*
 * A258923  E.g.f. satisfies A(x) = 1/(4 - 3*exp(x*A(x))).
 * https://oeis.org/A258923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)!) * Σ_{k=0..n} 3^k * (n+k)! * S(n,k) where
 *            S(n,k) are Stirling numbers of the second kind. The algorithm
 *            builds the Stirling numbers row by row and evaluates the sum
 *            using only integer arithmetic
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>
#endif

/* static tables and state */
static seq_t *S_prev = NULL;
static seq_t *S_curr = NULL;
static seq_t *A_arr  = NULL;
static idx_t  computed_max = 0;   /* largest n that has been computed */
static idx_t  capacity     = 0;   /* current table capacity */

/* ensure tables are large enough for indices up to 'needed' (inclusive) */
static void ensure_capacity(idx_t needed)
{
    if (capacity >= needed) return;

    idx_t new_cap = capacity ? capacity << 1 : 1;
    while (new_cap < needed) new_cap <<= 1;

    seq_t *new_prev = NULL;
    seq_t *new_curr = NULL;
    seq_t *new_arr  = NULL;

#ifdef __cplusplus
    new_prev = new seq_t[new_cap];
    new_curr = new seq_t[new_cap];
    new_arr  = new seq_t[new_cap];
#else
    new_prev = (seq_t*)malloc(new_cap * sizeof(seq_t));
    new_curr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    new_arr  = (seq_t*)malloc(new_cap * sizeof(seq_t));
#endif

    /* copy existing data */
    for (idx_t i = 0; i <= computed_max; ++i) {
        new_prev[i] = S_prev[i];
        new_arr[i]  = A_arr[i];
    }

    /* free old tables */
#ifdef __cplusplus
    delete[] S_prev;
    delete[] S_curr;
    delete[] A_arr;
#else
    free(S_prev);
    free(S_curr);
    free(A_arr);
#endif

    S_prev   = new_prev;
    S_curr   = new_curr;
    A_arr    = new_arr;
    capacity = new_cap;
}

/* public function required by the harness */
seq_t a(idx_t n)
{
    /* initialise tables on first use */
    if (!S_prev) {
#ifdef __cplusplus
        S_prev = new seq_t[1];
        S_curr = new seq_t[1];
        A_arr  = new seq_t[1];
#else
        S_prev = (seq_t*)malloc(1 * sizeof(seq_t));
        S_curr = (seq_t*)malloc(1 * sizeof(seq_t));
        A_arr  = (seq_t*)malloc(1 * sizeof(seq_t));
#endif
        capacity = 1;
        S_prev[0] = (seq_t)1;   /* S(0,0) = 1 */
        A_arr[0]  = (seq_t)1;   /* a(0) = 1 */
        computed_max = 0;
    }

    if (n > capacity - 1) {
        /* grow tables to accommodate requested n */
        ensure_capacity(n + 1);
    }

    if (n <= computed_max) {
        /* value already computed */
        return A_arr[n];
    }

    /* compute missing rows up to n */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        /* build Stirling row i using the recurrence
           S(i,k) = k*S(i-1,k) + S(i-1,k-1),  S(i,0)=0,  S(i,i)=1 */
        S_curr[0] = (seq_t)0;
        S_curr[i] = (seq_t)1;
        for (idx_t j = i - 1; j >= 1; --j) {
            S_curr[j] = (seq_t)j * S_prev[j] + S_prev[j - 1];
        }

        /* evaluate the sum for a(i)
           a(i) = Σ_{k=1..i} 3^k * S(i,k) * Π_{t=1}^{k-1} (i + t + 1)  */
        seq_t sum   = (seq_t)0;
        seq_t prod  = (seq_t)1;          /* product Π_{t=1}^{k-1} (i+t+1) */
        seq_t pow3  = (seq_t)3;          /* 3^k */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = pow3 * S_curr[k] * prod;
            sum += term;

            pow3 *= (seq_t)3;
            if (k < i) {
                prod *= (seq_t)(i + k + 1);
            }
        }

        A_arr[i] = sum;

        /* swap current and previous Stirling tables for next iteration */
        seq_t *tmp = S_prev;
        S_prev = S_curr;
        S_curr = tmp;
    }

    computed_max = n;
    return A_arr[n];
}
