/*
 * A375660  Expansion of e.g.f. 1 / (1 - x * (exp(x) - 1))^2.
 * https://oeis.org/A375660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/2)} (k+1)! *
 *            Stirling2(n-k,k)/(n-k)!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   mul_small, seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* utilities */
#include "../utils/seq_mul.h"     /* seq_mul(seq_t a, seq_t b) */
#include "../utils/mul_small.h"   /* mul_small(seq_t a, unsigned int m) */
#include "../utils/seq_realloc.h" /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

seq_t a(idx_t n)
{
    /* static cache for h (coefficients of 1/(1 - x*(e^x-1))) and a */
    static seq_t *h    = NULL;
    static seq_t *a_arr = NULL;
    static size_t h_cap = 0;
    static size_t a_cap = 0;
    static idx_t  computed = 0;   /* highest n that has been processed      */

    /* --- ensure capacity --- */
    size_t need = n + 1;
    if (h_cap < need) {
        size_t new_cap = (h_cap == 0) ? 1 : h_cap << 1;
        if (new_cap < need) new_cap = need;
        h = seq_realloc(h, h_cap, new_cap);
        h_cap = new_cap;
    }
    if (a_cap < need) {
        size_t new_cap = (a_cap == 0) ? 1 : a_cap << 1;
        if (new_cap < need) new_cap = need;
        a_arr = seq_realloc(a_arr, a_cap, new_cap);
        a_cap = new_cap;
    }

    /* --- base cases --- */
    if (computed == 0) {
        h[0]    = 1;     /* h_0 = 1   (coefficient of 1)           */
        a_arr[0] = 1;    /* a_0 = 1                                              */
        computed = 0;
    }

    /* --- fill tables up to n --- */
    for (idx_t i = computed + 1; i <= n; ++i) {

        /* compute h[i]  (coefficients of 1/(1 - x*(e^x-1))) */
        if (i == 1) {
            h[1] = 0;   /* h_1 = 0 */
        } else if (i >= 2) {
            seq_t sum = 0;
            seq_t bin = 1;   /* binom(i,0) */

            for (idx_t k = 0; k <= i - 2; ++k) {
                seq_t tmp   = mul_small(h[k], (unsigned int)(i - k));
                seq_t term  = seq_mul(bin, tmp);
                sum += term;

                if (k < i - 2) {
                    bin = mul_small(bin, (unsigned int)(i - k));
                    bin = bin / (seq_t)(k + 1);
                }
            }
            h[i] = sum;
        }

        /* compute a[i] = sum_{j=0..i} C(i,j) * h[j] * h[i-j] */
        seq_t sum_adj = 0;
        seq_t bin     = 1;   /* binom(i,0) */

        for (idx_t j = 0; j <= i; ++j) {
            seq_t prod  = seq_mul(h[j], h[i - j]);
            seq_t term  = seq_mul(bin, prod);
            sum_adj += term;

            if (j < i) {
                bin = mul_small(bin, (unsigned int)(i - j));
                bin = bin / (seq_t)(j + 1);
            }
        }
        a_arr[i] = sum_adj;
    }

    computed = n;
    return a_arr[n];
}
