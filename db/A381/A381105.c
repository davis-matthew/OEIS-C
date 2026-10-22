/*
 * A381105  Expansion of e.g.f. log(1-x)^2 * (exp(x) - 1) / 2.
 * https://oeis.org/A381105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n-1} binomial(n,k) * |Stirling1(k,2)|
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0) = 0, a(1) = 0, a(2) = 0 */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* ---------- Stirling1(k,2) (unsigned) ---------- */
static seq_t *S = NULL;        /* S[k] = |Stirling1(k,2)|, k >= 0 */
static idx_t S_limit = 0;      /* number of computed elements in S */
static seq_t fact_nm2 = 1;     /* holds (k-2)! for the next k to be computed */

/* Ensure S is computed up to 'up_to' (inclusive) */
static void ensure_S(idx_t up_to)
{
    if (S_limit > 0 && up_to < S_limit) return;

    size_t new_cap = up_to + 1;
    seq_t *new_S = alloc_seq(new_cap);
    if (S != NULL) {
        for (idx_t i = 0; i < S_limit; ++i)
            new_S[i] = S[i];
        free_seq(S);
    }
    S = new_S;

    /* Initialise the first two entries if needed */
    if (S_limit == 0) {
        S[0] = 0;
        if (new_cap > 1) S[1] = 0;
        S_limit = 2;
        fact_nm2 = 1;           /* (2-2)! = 0! for the next k = 2 */
    } else if (S_limit == 1) {
        S[1] = 0;
        S_limit = 2;
        fact_nm2 = 1;
    }

    /* Compute the remaining Stirling numbers */
    for (idx_t k = S_limit; k <= up_to; ++k) {
        /* k >= 2 :  s(k,2) = (k-1)*s(k-1,2) + (k-2)!  */
        seq_t value = (seq_t)(k - 1) * S[k - 1] + fact_nm2;
        S[k] = value;
        /* Update factorial for next iteration: (k-1)! */
        fact_nm2 = fact_nm2 * (seq_t)(k - 1);
    }
    S_limit = up_to + 1;
}

/* ---------- Sequence a(n) ---------- */
static seq_t *A = NULL;        /* memoised a(n) values */
static idx_t A_limit = 0;      /* number of computed elements in A */

/* Ensure A is computed up to 'up_to' (inclusive) */
static void ensure_A(idx_t up_to)
{
    if (A_limit > 0 && up_to < A_limit) return;

    size_t new_cap = up_to + 1;
    seq_t *new_A = alloc_seq(new_cap);
    if (A != NULL) {
        for (idx_t i = 0; i < A_limit; ++i)
            new_A[i] = A[i];
        free_seq(A);
    }
    A = new_A;

    for (idx_t n = A_limit; n <= up_to; ++n) {
        if (n <= 2) {
            A[n] = 0;
            continue;
        }

        /* Compute a(n) = sum_{k=2}^{n-1} binom(n,k) * S[k] */
        ensure_S(n - 1);

        seq_t sum = 0;
        seq_t binom = 1;          /* binom(n,0) = 1 */

        for (idx_t k = 0; k < n; ++k) {
            if (k >= 2) {
                sum += binom * S[k];
            }
            /* Prepare next binomial value */
            seq_t mult = binom * (seq_t)(n - k);
            binom = mult / (seq_t)(k + 1);   /* exact division */
        }

        A[n] = sum;
    }

    A_limit = up_to + 1;
}

/* Public interface */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    ensure_A(n);
    return A[n];
}
