/*
 * A368441  Expansion of e.g.f. exp(x) / (4 - 3*exp(3*x)).
 * https://oeis.org/A368441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + 3 * Sum_{k=1..n} 3^k * binomial(n,k) * a(n-k) Initial
 *            value: a(0) = 1 Recurrence is computed iteratively using
 *            convolution with binomial coefficients and powers of 3. The
 *            function is memoised so that subsequent calls, irrespective of
 *            order, return the correct value
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Ensure capacity for a seq_t array */
static inline void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elsize) {
    if (*cap < needed) {
        /* Use standard realloc for portability; zero‑initialise new part */
        void *tmp = realloc(*ptr, needed * elsize);
        if (!tmp) return; /* allocation failure – caller will fail gracefully */
        *ptr = tmp;
        *cap = needed;
    }
}

/* The sequence function */
seq_t a(idx_t n)
{
    /* ---------- cache for a(n) values ---------- */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t last_computed = 0;          /* highest index computed so far */

    ensure_capacity((size_t)n + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Initialise a(0) if necessary */
    if (n == 0) {
        cache[0] = 1;
        return cache[0];
    }
    if (cache[0] == 0) cache[0] = 1;

    if (n <= last_computed) return cache[n];

    /* ---------- cache for powers of 3: pow3[i] = 3^i ---------- */
    static seq_t *pow3 = NULL;
    static size_t pow3_cap = 0;
    static idx_t pow3_last = 0;           /* index up to which pow3 is filled */

    if (!pow3) {
        ensure_capacity(1, &pow3_cap, (void **)&pow3, sizeof(seq_t));
        pow3[0] = 1;
        pow3_last = 0;
    }
    if (pow3_cap <= (size_t)n) {
        ensure_capacity((size_t)n + 1, &pow3_cap, (void **)&pow3, sizeof(seq_t));
        for (idx_t i = pow3_last + 1; i <= n; ++i) {
            pow3[i] = pow3[i - 1] * 3;
        }
        pow3_last = n;
    }

    /* ---------- compute a(i) for i = last_computed+1 .. n ---------- */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;          /* C(i,0) */
        for (idx_t k = 0; k < i; ++k) {
            /* term = a(k) * bin * 3^{i-k} */
            seq_t term = cache[k];
            term = term * bin;
            term = term * pow3[i - k];
            sum += term;

            /* update binomial coefficient for next k: C(i,k+1) */
            if (k + 1 < i) {
                seq_t num = bin * (seq_t)(i - k);
                bin = num / (seq_t)(k + 1);
            }
        }
        cache[i] = 1 + (sum * 3);
    }
    last_computed = n;
    return cache[n];
}
