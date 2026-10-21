/*
 * A275488  Number of labeled forests of (free) trees such that exactly one
 *          tree is a path.
 * https://oeis.org/A275488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{p=1}^n B(p) * C(n,p) * F(n‑p) where B(p) = 1 (p≤2)
 *            else p! / 2 – path‑trees U(j) = j^(j‑2) – B(j) – trees that are
 *            NOT paths F(t) = Σ_{j=1}^t C(t‑1,j‑1) * U(j) * F(t‑j) – forests
 *            with no path component a(n) = Σ_{p=1}^n B(p) * C(n,p) * F(n‑p) –
 *            exactly one path component
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/mul_small.h" /* seq_t mul_small(seq_t x, unsigned int m); */

seq_t a(idx_t n)
{
    static idx_t capacity = 0;   /* maximum index currently allocated +1 */
    static idx_t computed = 0;   /* largest n already processed */

    /* Arrays holding precomputed values */
    static seq_t *fact = NULL;   /* fact[k] = k!          */
    static seq_t *B    = NULL;   /* B[k]  = path‑tree count  */
    static seq_t *U    = NULL;   /* U[k]  = non‑path tree count  */
    static seq_t *F    = NULL;   /* F[k]  = forest count with no path component  */
    static seq_t *RES  = NULL;   /* RES[k] = desired a(k)  */

    /* Helper to grow an array to new_cap (>= old_cap+1) */
    if (n >= capacity) {
        idx_t new_cap = n + 1;     /* indices 0..n inclusive */

        /* Reallocate each array, keeping the old data */
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        B    = (seq_t *)realloc(B,    new_cap * sizeof(seq_t));
        U    = (seq_t *)realloc(U,    new_cap * sizeof(seq_t));
        F    = (seq_t *)realloc(F,    new_cap * sizeof(seq_t));
        RES  = (seq_t *)realloc(RES,  new_cap * sizeof(seq_t));
        capacity = new_cap;

        /* Base values for factorials and the empty forest */
        if (computed == 0) {
            fact[0] = (seq_t)1;
            F[0]    = (seq_t)1;
        }
    }

    /* Compute values for all indices > computed up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        /* 1. factorials */
        fact[k] = mul_small(fact[k - 1], (unsigned int)k);

        /* 2. path‑tree counts B(k) */
        if (k <= 2)
            B[k] = (seq_t)1;
        else
            B[k] = fact[k] / (seq_t)2;

        /* 3. non‑path tree counts U(k) = k^(k-2) – B(k) */
        if (k <= 2)
            U[k] = (seq_t)0;
        else {
            seq_t pow_val = big_pow((seq_t)k, (seq_t)(k - 2));
            U[k] = pow_val - B[k];
        }

        /* 4. forests with no path component: F(k) */
        {
            seq_t sum = (seq_t)0;
            seq_t comb = (seq_t)1;          /* C(k-1,0) */
            for (idx_t j = 1; j <= k; ++j) {
                if (U[j])
                    sum += comb * U[j] * F[k - j];
                if (j < k)
                    comb = comb * (seq_t)(k - j) / (seq_t)j;
            }
            F[k] = sum;
        }

        /* 5. final sequence values a(k) */
        {
            seq_t sum = (seq_t)0;
            seq_t comb = (seq_t)k;          /* C(k,1) */
            for (idx_t p = 1; p <= k; ++p) {
                if (B[p])
                    sum += B[p] * comb * F[k - p];
                if (p < k)
                    comb = comb * (seq_t)(k - p) / (seq_t)(p + 1);
            }
            RES[k] = sum;
        }
    }
    computed = n;   /* all terms up to n are now ready */

    return RES[n];
}
