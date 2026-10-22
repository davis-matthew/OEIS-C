/*
 * A394948  Number of equivalence classes of well-formed bracketed words of
 *          total length 2n built from the symbol * (of length 2) and two
 *          unary bracket types () and [], with no empty bracket pair, modulo
 *          commutativity of concatenation.
 * https://oeis.org/A394948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A394948
   Formula: a(0)=1, a(1)=1,
            for n>1: n*a(n) = c(n) + ∑_{k=1}^{n-1} c(k)*a(n-k),
            c(n) = ∑_{m|n} m*b(m),  b(1)=1, b(m)=2*a(m-1) for m≥2   */

#include <stdlib.h>
#include <stdint.h>

/* static helper to grow a cache array to hold indices 0..n */
static void ensure_capacity(idx_t n, seq_t **cache, size_t *cap)
{
    size_t needed = (size_t)n + 1;
    if (*cap <= needed) {
        size_t new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(*cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);              /* abort on allocation failure */
        *cache = tmp;
        for (size_t i = *cap; i < new_cap; ++i)
            (*cache)[i] = 0;            /* zero‑initialize new slots */
        *cap = new_cap;
    }
}

/* external linkage as required */
seq_t a(idx_t n)
{
    static seq_t *a_cache = NULL;
    static seq_t *c_cache = NULL;
    static size_t a_cap = 0;
    static size_t c_cap = 0;
    static idx_t computed = 0;          /* largest n computed so far */

    /* Ensure caches can hold index n */
    ensure_capacity(n, &a_cache, &a_cap);
    ensure_capacity(n, &c_cache, &c_cap);

    /* Initialisation for n==0 or 1 */
    if (computed == 0) {
        a_cache[0] = 1;
        c_cache[0] = 0;                  /* unused */
        if (n >= 1) {
            a_cache[1] = 1;
            c_cache[1] = 1;              /* 1*1 */
        }
        computed = (n >= 1 ? 1 : 0);
    }

    /* Return cached value if already computed */
    if (n <= computed)
        return a_cache[n];

    /* Compute values up to index n */
    for (idx_t i = computed + 1; i <= n; ++i) {

        /* compute c(i) = Σ_{m|i} m*b(m) */
        seq_t sum_c = 0;
        for (idx_t d = 1; d * d <= i; ++d) {
            if (i % d == 0) {
                /* divisor d */
                seq_t term;
                if (d == 1)
                    term = 1;                     /* b(1)=1 */
                else
                    term = (seq_t)d * (seq_t)2 * a_cache[d - 1];
                sum_c += term;

                idx_t e = i / d;                  /* paired divisor */
                if (e != d) {
                    seq_t term2;
                    if (e == 1)
                        term2 = 1;
                    else
                        term2 = (seq_t)e * (seq_t)2 * a_cache[e - 1];
                    sum_c += term2;
                }
            }
        }
        c_cache[i] = sum_c;

        /* compute rhs = c(i) + Σ_{k=1}^{i-1} c(k)*a(i-k) */
        seq_t sum_over = 0;
        for (idx_t k = 1; k < i; ++k)
            sum_over += c_cache[k] * a_cache[i - k];

        seq_t rhs = c_cache[i] + sum_over;
        a_cache[i] = rhs / (seq_t)i;          /* division is exact */
    }

    computed = n;
    return a_cache[n];
}
