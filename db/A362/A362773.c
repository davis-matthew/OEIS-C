/*
 * A362773  E.g.f. satisfies A(x) = exp( x * (1+x) * A(x)^2 ).
 * https://oeis.org/A362773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * * Σ_{k=0..n} (2k+1)^(k-1) * binomial(k, n-k) / k! *
 *            * Recurrence: Let p_k = (2k+1)^(k-1) (p_0 = 1). * For each n, *
 *            a(n) = Σ_{m=0..⌊n/2⌋} p_{n-m} * T_m * where T_0 = 1, * T_{m+1} =
 *            T_m * (n-2m) * (n-2m-1) / (m+1). * Each a(n) is computed in O(n)
 *            time after pre‑computing the needed * p...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"
#include "../utils/to_seq.h"

/* ---------- Cache for a(n) values ---------- */
static seq_t *A_cache = NULL;   /* array of cached a(n) */
static size_t A_cap   = 0;      /* allocated capacity */
static idx_t  A_len   = 0;      /* number of values already computed */

/* Ensure A_cache can hold indices up to idx (inclusive). */
static void ensure_a_cache(idx_t idx)
{
    size_t needed = (size_t)idx + 1;
    if (needed > A_cap) {
        seq_t *tmp = (seq_t *)realloc(A_cache, needed * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        A_cache = tmp;
        A_cap   = needed;
    }
}

/* ---------- Cache for p_k = (2k+1)^(k-1) ---------- */
static seq_t *P_cache = NULL;   /* array of cached p_k */
static size_t P_cap   = 0;      /* number of p values stored */

/* Ensure P_cache can hold indices up to idx (inclusive). */
static void ensure_p_cache(idx_t idx)
{
    size_t needed = (size_t)idx + 1;
    if (needed > P_cap) {
        seq_t *tmp = (seq_t *)realloc(P_cache, needed * sizeof(seq_t));
        if (!tmp) abort();          /* allocation failure */
        P_cache = tmp;
        for (size_t i = P_cap; i < needed; ++i) {
            if (i == 0)
                P_cache[i] = (seq_t)1;
            else {
                seq_t base = to_seq(2 * i + 1);
                seq_t exp  = to_seq(i - 1);
                P_cache[i] = big_pow(base, exp);
            }
        }
        P_cap = needed;
    }
}

/* Compute all a values up to n that have not yet been cached. */
static void compute_up_to(idx_t n)
{
    if (n < A_len) return;            /* already computed up to n */

    ensure_p_cache(n);                /* all needed p_k available */
    ensure_a_cache(n);                /* ensure array capacity for all indices */

    for (idx_t i = A_len; i <= n; ++i) {
        size_t mmax = (size_t)(i >> 1);  /* floor(i/2) */
        seq_t temp = (seq_t)1;           /* T_0 for this i */
        seq_t sum  = (seq_t)0;

        for (size_t m = 0; m <= mmax; ++m) {
            seq_t p_val = P_cache[i - m];
            sum += p_val * temp;

            if (m == mmax) break;           /* no next T_m */

            /* Update T_{m+1} = T_m * (i-2m) * (i-2m-1) / (m+1) */
            seq_t f1 = to_seq(i - 2 * (idx_t)m);
            seq_t f2 = f1 - (seq_t)1;        /* i-2m-1 */
            temp = temp * f1 * f2;
            temp = temp / to_seq((idx_t)m + 1);
        }
        A_cache[i] = sum;
    }
    A_len = n + 1;
}

/* Public entry point: returns a(n) for any n >= 0 */
seq_t a(idx_t n)
{
    if (!A_cache) {                           /* first call: allocate */
        A_cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)(n + 1));
        if (!A_cache) abort();
        A_cap = n + 1;
    }
    compute_up_to(n);
    return A_cache[n];
}
