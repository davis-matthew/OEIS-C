/*
 * A367828  E.g.f. A(x) satisfies A(x) = (1 - log(1 - x) * A(2*x)) / (1 - x).
 * https://oeis.org/A367828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A367828
   a(0) = 1
   a(n) = n * a(n-1) + Σ_{k=1..n} 2^(n-k) * (k-1)! * binomial(n,k) * a(n-k)  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ---- Sequence cache ----------------------------------------------- */
    static seq_t *a_cache = NULL;
    static idx_t a_cap  = 0;      /* allocated elements */
    static idx_t a_len  = 0;      /* number of terms computed (next free index) */

    /* ---- Factorial cache ---------------------------------------------- */
    static seq_t *fact = NULL;
    static idx_t fact_cap = 0;    /* allocated elements */
    static idx_t fact_len = 0;    /* number of factorials computed */

    /* ---- Powers of two cache ------------------------------------------ */
    static seq_t *pow2 = NULL;
    static idx_t pow2_cap = 0;    /* allocated elements */
    static idx_t pow2_len = 0;    /* number of powers of two computed */

    /* ---- Ensure cache capacity for a_cache --------------------------- */
    if (n + 1 > a_cap) {
        idx_t new_cap = a_cap ? a_cap << 1 : 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cap = new_cap;
    }

    /* ---- Initialise a_cache base case --------------------------------- */
    if (a_len == 0) {
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* ---- Ensure factorial cache capacity -------------------------------- */
    if (n + 1 > fact_cap) {
        idx_t new_cap = fact_cap ? fact_cap << 1 : 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        fact = tmp;
        fact_cap = new_cap;
    }

    if (fact_len == 0) {
        fact[0] = (seq_t)1;
        fact_len = 1;
    }
    /* Compute any missing factorials up to n */
    for (idx_t i = fact_len; i <= n; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;
    fact_len = n + 1;

    /* ---- Ensure powers of two cache capacity --------------------------- */
    if (n + 1 > pow2_cap) {
        idx_t new_cap = pow2_cap ? pow2_cap << 1 : 1;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(pow2, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        pow2 = tmp;
        pow2_cap = new_cap;
    }

    if (pow2_len == 0) {
        pow2[0] = (seq_t)1;
        pow2_len = 1;
    }
    for (idx_t i = pow2_len; i <= n; ++i)
        pow2[i] = pow2[i - 1] << 1;
    pow2_len = n + 1;

    /* ---- Compute terms up to n ---------------------------------------- */
    for (idx_t i = a_len; i <= n; ++i) {
        if (i == 0) {          /* base already set */
            a_cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = (seq_t)0;
        seq_t binom = 1;       /* will be updated to C(i,k) inside the loop */

        for (idx_t k = 1; k <= i; ++k) {
            /* binomial coefficient C(i,k) */
            binom = binom * (seq_t)(i - k + 1) / (seq_t)k;

            seq_t term = pow2[i - k];           /* 2^(i-k) */
            term     *= fact[k - 1];            /* (k-1)!   */
            term     *= binom;                  /* C(i,k)   */
            term     *= a_cache[i - k];         /* a(n-k)   */
            sum     += term;
        }

        seq_t prod = (seq_t)i * a_cache[i - 1];
        a_cache[i] = prod + sum;
    }
    a_len = n + 1;

    return a_cache[n];
}
