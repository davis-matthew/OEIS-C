/*
 * A394148  G.f. A(x) satisfies
 *          3*A(x)^6-7*A(x)^5+(9x+5)*A(x)^4-(11x+1)*A(x)^3+(6x^2+3x)*A(x)^2-3x^2*A(x)+x^3=0.
 * https://oeis.org/A394148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: 3*A(x)^6 - 7*A(x)^5 + (9x+5)*A(x)^4 - (11x+1)*A(x)^3
 *          + (6x^2+3x)*A(x)^2 - 3x^2*A(x) + x^3 = 0
 *
 * The sequence a(n) is the coefficient of x^n in the power series A(x)
 * satisfying the above equation, where A(0)=0.  It is computed
 * by solving the equivalent system of functional equations for the
 * generating functions of trees rooted at red (R), blue (B), and green (G) vertices:
 *
 *     R = x / (1 - (R + B))
 *     B = x / (1 - (R + B + G))
 *     G = x / (1 - (R + G))
 *
 * which expands to the recurrences
 *
 *     R = x + R^2 + R*B
 *     B = x + B*R + B^2 + B*G
 *     G = x + G*R + G^2
 *
 * The coefficient a(n) is the n-th coefficient of R.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 0) return (seq_t)0;

    /* Static vectors to cache computed terms */
    static seq_t *R = NULL, *B = NULL, *G = NULL;
    static size_t cap = 0;          /* allocated capacity (number of elements) */
    static size_t computed = 0;     /* largest n for which terms are known */

    /* Ensure we have storage for at least n+1 elements (index 0..n) */
    if (n + 1 > cap) {
        size_t new_cap = (n + 1) * 2;
        if (cap == 0) {
            cap = new_cap;
            R = (seq_t*)calloc(cap, sizeof(seq_t));
            B = (seq_t*)calloc(cap, sizeof(seq_t));
            G = (seq_t*)calloc(cap, sizeof(seq_t));
        } else {
            cap = new_cap;
            R = (seq_t*)realloc(R, cap * sizeof(seq_t));
            B = (seq_t*)realloc(B, cap * sizeof(seq_t));
            G = (seq_t*)realloc(G, cap * sizeof(seq_t));
            /* new elements are zeroed by realloc by standard? It does not guarantee.  */
            for (size_t i = computed + 1; i < cap; ++i) {
                R[i] = B[i] = G[i] = (seq_t)0;
            }
        }
    }

    /* Initialise base values once */
    if (computed == 0) {
        R[0] = B[0] = G[0] = (seq_t)0;
        R[1] = B[1] = G[1] = (seq_t)1;   /* first non‑zero term */
        computed = 1;
    }

    /* Compute terms up to n if needed */
    for (size_t m = computed + 1; m <= (size_t)n; ++m) {
        /* start with zero; contributions from the x term are only when m==1,
         * but m>1 here, so start at zero.  */
        R[m] = (seq_t)0;
        B[m] = (seq_t)0;
        G[m] = (seq_t)0;

        for (size_t i = 1; i < m; ++i) {
            size_t j = m - i;

            seq_t r_i = R[i], r_j = R[j];
            seq_t b_i = B[i], b_j = B[j];
            seq_t g_i = G[i], g_j = G[j];

            /* R = x + R^2 + R*B */
            R[m] += r_i * r_j + r_i * b_j;

            /* B = x + B*R + B^2 + B*G */
            B[m] += b_i * r_j + b_i * b_j + b_i * g_j;

            /* G = x + G*R + G^2 */
            G[m] += g_i * r_j + g_i * g_j;
        }
    }
    computed = (size_t)n;

    return R[n];
}
