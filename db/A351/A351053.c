/*
 * A351053  G.f. A(x) satisfies: A(x) = x + x^2 * A(x/(1 - 3*x)) / (1 - 3*x).
 * https://oeis.org/A351053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(n) = Σ_{k=0..n-2} binomial(n-2,k) * 3^k *
 *            a(n-k-2) for n ≥ 2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*------------------------------------------------------------*/
/*   Memoisation helpers                                     */
/*------------------------------------------------------------*/

static seq_t *a_cache = NULL;   /* a_cache[i] = a(i)        */
static idx_t  a_cap   = 0;       /* number of stored elements */

static seq_t *pow3_cache = NULL; /* pow3_cache[i] = 3^i      */
static idx_t  pow3_cap   = 0;    /* current size of pow3_cache */

/* Reallocation helper that is valid in C and C++ */
static void* safe_realloc(void* p, size_t n) {
#ifdef __cplusplus
    return static_cast<void*>(realloc(p, n));
#else
    return realloc(p, n);
#endif
}

/* Ensure pow3_cache contains powers of 3 up to exp; grow if needed */
static void ensure_pow3(idx_t exp) {
    if (exp < pow3_cap)
        return;

    size_t new_cap = exp + 1;
    pow3_cache = (seq_t*)safe_realloc(pow3_cache, new_cap * sizeof(seq_t));
    /* Initialise newly allocated part */
    for (idx_t i = pow3_cap; i < new_cap; ++i) {
        if (i == 0)
            pow3_cache[0] = 1;
        else
            pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
    }
    pow3_cap = new_cap;
}

/* Ensure a_cache contains terms a(0)..a(n); grow if needed          */
static void ensure_a(idx_t n) {
    if (n < a_cap)
        return;

    /* Need powers of 3 up to exponent n-2 for recurrence */
    if (n >= 2)
        ensure_pow3(n - 2);

    size_t old = a_cap;
    a_cap = n + 1;
    a_cache = (seq_t*)safe_realloc(a_cache, a_cap * sizeof(seq_t));

    /* Initialise first missing elements if any */
    if (old == 0) {
        a_cache[0] = 0;
        old = 1;
        if (n >= 1) {
            a_cache[1] = 1;
            old = 2;
        }
    } else if (old == 1 && n >= 1) {
        a_cache[1] = 1;
        old = 2;
    }

    /* Compute missing terms */
    for (idx_t i = old; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = 0;
        } else if (i == 1) {
            a_cache[1] = 1;
        } else {
            idx_t m = i - 2;               /* m = n-2 in the recurrence   */
            seq_t sum = 0;
            seq_t bin = 1;                 /* C(m,0)                       */
            for (idx_t k = 0; k <= m; ++k) {
                seq_t term = bin;
                term *= pow3_cache[k];                     /* *3^k  */
                term *= a_cache[i - k - 2];               /* *a[n-k-2] */
                sum += term;
                if (k < m) {
                    bin = bin * (seq_t)(m - k);
                    bin = bin / (seq_t)(k + 1);
                }
            }
            a_cache[i] = sum;
        }
    }
}

/*------------------------------------------------------------*/
/*   Public function                                          */
/*------------------------------------------------------------*/

seq_t a(idx_t n) {
    ensure_a(n);
    return a_cache[n];
}
