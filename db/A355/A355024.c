/*
 * A355024  Number of unlabeled trees on n nodes with maximum degree three and
 *          three vertices of degree three.
 * https://oeis.org/A355024
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* formula: a(n) are the coefficients of z^n in
   z^8 * (1 - z + 2 z^2) / ((1 - z)^7 (1 + z)^3 (1 + z^2)) */
#include <stdbool.h>
#include <stdlib.h>

typedef struct { seq_t mag; bool neg; } Sig;

/* initialise a zero-signed big integer */
static inline void sig_zero(Sig *s) { s->mag = 0; s->neg = false; }

/* add two signed integers: res = a + b */
static inline void sig_add(Sig *res, const Sig *a, const Sig *b) {
    if (a->neg == b->neg) {                /* same sign → add magnitudes */
        res->mag = a->mag + b->mag;
        res->neg = a->neg;
    } else {                               /* different sign → subtract magnitudes */
        if (a->mag > b->mag) {
            res->mag = a->mag - b->mag;
            res->neg = a->neg;
        } else if (b->mag > a->mag) {
            res->mag = b->mag - a->mag;
            res->neg = b->neg;
        } else {                           /* equal magnitude → zero */
            res->mag = 0;
            res->neg = false;
        }
    }
}

/* add a signed term to an existing sum: sum += val * (neg ? -1 : +1) */
static inline void sig_add_assign(Sig *sum, seq_t val, bool neg) {
    Sig term = { val, neg };
    Sig tmp; sig_add(&tmp, sum, &term); *sum = tmp;
}

/* subtract two signed integers: res = a - b */
static inline void sig_sub(Sig *res, const Sig *a, const Sig *b) {
    Sig negb = { b->mag, !b->neg };  /* -b */
    sig_add(res, a, &negb);
}

/* small binomial coefficient for n <= 7, returns int */
static inline int binom_small(int n, int k) {
    if (k < 0 || k > n) return 0;
    if (k > n - k) k = n - k;
    int r = 1;
    for (int i = 1; i <= k; ++i) r = r * (n - k + i) / i;
    return r;
}

/* coefficients of the denominator polynomial D(z) = (1 - z)^7 (1 + z)^3 (1 + z^2) */
static void init_den_coeff(int d[13]) {
    long long c[11] = {0};                 /* (1 - z)^7 * (1 + z)^3   -> degree 10 */
    for (int i = 0; i <= 7; ++i) {
        long long ci = binom_small(7, i);
        if (i & 1) ci = -ci;
        for (int j = 0; j <= 3; ++j) {
            long long cj = binom_small(3, j);
            c[i + j] += ci * cj;
        }
    }
    /* multiply by (1 + z^2) */
    for (int k = 0; k <= 12; ++k) {
        long long a = (k <= 10) ? c[k] : 0;
        long long b = (k >= 2) ? c[k - 2] : 0;
        d[k] = (int)(a + b);              /* exact integers in the range [-something, something] */
    }
}

/* cache for computed values */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_max = 0;

/* recurrence coefficients */
static bool d_inited = false;
static int d_coeff[13] = {0};

seq_t a(idx_t n) {
    if (n < 8) return 0;

    /* initialise denominator coefficients once */
    if (!d_inited) {
        init_den_coeff(d_coeff);
        d_inited = true;
    }

    /* expand cache if needed */
    size_t needed = (size_t)n + 1;
    if (needed > cache_cap) {
        size_t new_cap = needed;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;                /* allocation failure */
        if (cache) {
            for (size_t i = 0; i < cache_cap; ++i) new_cache[i] = cache[i];
        }
        for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms up to n */
    idx_t start = cache_max + 1;
    if (start < 8) start = 8;
    for (idx_t idx = start; idx <= n; ++idx) {
        /* compute the linear recurrence:
           a(idx) = -sum_{k=1..12} d_k * a(idx-k) + b_idx
           where b_idx = 1 if idx==8,
                      -1 if idx==9,
                       2 if idx==10,
                       0 otherwise
        */
        Sig sum; sig_zero(&sum);
        for (int k = 1; k <= 12; ++k) {
            if (k > idx) continue;            /* indices below 8 are zero */
            seq_t val = cache[idx - k];
            int coeff = d_coeff[k];
            if (coeff == 0 || val == 0) continue;
            seq_t prod = val * (seq_t)(coeff >= 0 ? coeff : -coeff);
            bool prod_neg = (coeff < 0);
            sig_add_assign(&sum, prod, prod_neg);
        }

        /* right‑hand side b_idx */
        int rhs_val = 0;
        if (idx == 8) rhs_val = 1;
        else if (idx == 9) rhs_val = -1;
        else if (idx == 10) rhs_val = 2;
        Sig rhs = { (seq_t)(rhs_val >= 0 ? rhs_val : -rhs_val), rhs_val < 0 };
        Sig res;
        sig_sub(&res, &rhs, &sum);          /* res = rhs - sum */

        /* ensure result is non‑negative */
        cache[idx] = res.neg ? 0 : res.mag;
    }
    cache_max = n;
    return cache[n];
}
