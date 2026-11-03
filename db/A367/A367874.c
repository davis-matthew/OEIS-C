/*
 * A367874  Expansion of e.g.f. exp(x * (2 + exp(x))).
 * https://oeis.org/A367874
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0..n} (k+2)^(n-k) * binomial(n,k) Recurrence: a(0) =
 *            1 a(n) = 2*a(n-1) + ∑_{k=1..n} binomial(n-1,k-1) * k * a(n-k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t up_to = 0;

    /* initialisation */
    if (!cache) {
        cap = 1;
        cache = seq_alloc(cap);
        cache[0] = (seq_t)1;   /* a(0) */
        up_to = 0;
    }

    /* extend cache up to n */
    if (n > up_to) {
        /* grow capacity if necessary */
        if (cap < (size_t)(n + 1)) {
            size_t new_cap = cap * 2;
            if (new_cap < (size_t)(n + 1))
                new_cap = n + 1;
            cache = seq_realloc(cache, cap, new_cap);
            cap = new_cap;
        }

        /* compute terms from up_to+1 to n */
        for (idx_t i = up_to + 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            seq_t bin  = (seq_t)1; /* binomial(i-1,0) */

            for (idx_t k = 1; k <= i; ++k) {
                /* term = binomial(i-1,k-1) * k * a(i-k) */
                seq_t term = bin * (seq_t)k * cache[i - k];
                sum += term;

                if (k < i) {
                    /* update bin to binomial(i-1,k) */
                    bin = bin * (seq_t)(i - k);
                    bin = bin / (seq_t)k; /* exact division */
                }
            }

            cache[i] = (seq_t)2 * cache[i - 1] + sum;
        }

        up_to = n;
    }

    return cache[n];
}
