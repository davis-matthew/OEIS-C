/*
 * A344398  a(n) = (-1)^n * F_{n}((-1)^n * n), where F_{n}(x) is the Fubini
 *          polynomial.
 * https://oeis.org/A344398
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^n * F_n((-1)^n * n), F_n(x) = Σ_{k=0}^n k! · S(n,k)
 *            · x^k
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* External integer types (provided by the build). */
extern seq_t  to_seq_t(idx_t v);
extern idx_t  to_idx_t(seq_t v);

/* Per‑call caches. */
static seq_t *a_cache  = NULL;  /* a(n) values */
static seq_t *S_prev   = NULL;  /* row n‑1 of Stirling numbers */
static seq_t *S_cur    = NULL;  /* row n   of Stirling numbers */
static size_t a_cap   = 0;      /* cache size for a_cache */
static size_t S_cap   = 0;      /* current array capacity for S_prev/S_cur */
static idx_t  max_n   = 0;      /* largest n already computed */

/* Ensure an array has at least new_cap elements. */
static void ensure_capacity(size_t new_cap, seq_t **ptr, size_t *cap)
{
    if (*cap < new_cap) {
        seq_t *tmp = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* --- first‑time initialisation ------------------------------------- */
    if (a_cache == NULL) {
        a_cap   = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) exit(EXIT_FAILURE);
        a_cache[0] = (seq_t)1;   /* a(0) = 1 */
        max_n = 0;

        /* allocate Stirling rows (short‑initial values). */
        S_cap   = 1;
        S_prev  = (seq_t *)malloc(S_cap * sizeof(seq_t));
        S_cur   = (seq_t *)malloc(S_cap * sizeof(seq_t));
        if (!S_prev || !S_cur) exit(EXIT_FAILURE);
        S_prev[0] = (seq_t)1;   /* S(0,0) = 1 */
    }

    /* --- extend the cache if needed ------------------------------------ */
    if ((size_t)n + 1 > a_cap)
        ensure_capacity((size_t)n + 1, &a_cache, &a_cap);

    /* --- compute a(k) for k = max_n+1 … n ----------------------------- */
    for (idx_t cur = max_n + 1; cur <= n; ++cur) {
        /* ensure both Stirling arrays have space for cur+1 elements. */
        ensure_capacity((size_t)cur + 1, &S_cur,  &S_cap);
        ensure_capacity((size_t)cur + 1, &S_prev, &S_cap);

        /* ----- compute Stirling numbers of the second kind for this row */
        S_cur[0] = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            seq_t term1 = S_prev[k - 1];
            seq_t term2 = (k <= cur - 1) ? S_prev[k] * (seq_t)k : (seq_t)0;
            S_cur[k] = term1 + term2;
        }

        /* ----- evaluate the signed sum for F_n(x) with x = (-1)^n * n --- */
        seq_t fact    = (seq_t)1;   /* k! */
        seq_t pow_n   = (seq_t)1;   /* n^k */
        seq_t sum_pos = (seq_t)0;   /* even‑k terms when n is odd, all terms when n is even */
        seq_t sum_neg = (seq_t)0;   /* odd‑k terms when n is odd */

        int odd = (cur & 1u);

        for (idx_t k = 0; k <= cur; ++k) {
            seq_t term = S_cur[k] * fact * pow_n;
            if (!odd) {
                sum_pos += term;  /* n even: all terms positive */
            } else {
                if ((k & 1u) == 0)
                    sum_pos += term;  /* even k (sign −1) */
                else
                    sum_neg += term;  /* odd  k (sign +1) */
            }
            fact *= (seq_t)(k + 1);
            pow_n *= (seq_t)cur;
        }

        seq_t val;
        if (!odd) {
            val = sum_pos;
        } else {
            /* a(n) = sum_neg − sum_pos  (for odd n) */
            val = sum_neg - sum_pos;
        }

        a_cache[cur] = val;

        /* Prepare for next iteration: swap rows. */
        seq_t *tmp  = S_prev;
        S_prev      = S_cur;
        S_cur       = tmp;
    }

    max_n = n;
    return a_cache[n];
}
