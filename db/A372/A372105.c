/*
 * A372105  G.f. A(x) satisfies A(x) = 1/( 1 - 9*x*A(x)/(1-x) )^(1/3).
 * https://oeis.org/A372105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 9^k * binomial(4*k/3-2/3,k) *
 *            binomial(n-1,n-k)/(k+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the functional equation     *
 * (1-x)(A^3-1) = 9xA^4 to compute the coefficients   *
 * of the generating function by dynamic programming.   */

#include <stdlib.h>
#include <stddef.h>

/* public function required by the judge */
seq_t a(idx_t n)
{
    /* static storage for the sequence and auxiliary arrays */
    static seq_t *seq  = NULL;   /* a[n]                */
    static seq_t *d    = NULL;   /* convolution a*a    */
    static seq_t *b    = NULL;   /* convolution a*d    */
    static seq_t *c    = NULL;   /* convolution a*b    */
    static size_t capacity = 0;
    static idx_t   max_comp = 0;

    /* initialise on first call */
    if (seq == NULL) {
        capacity = 16;   /* initial capacity (number of stored terms) */
        seq = (seq_t*)malloc(capacity * sizeof(seq_t));
        d   = (seq_t*)malloc(capacity * sizeof(seq_t));
        b   = (seq_t*)malloc(capacity * sizeof(seq_t));
        c   = (seq_t*)malloc(capacity * sizeof(seq_t));

        /* term n = 0 */
        seq[0] = 1;      /* a[0] = 1 */
        d[0]   = 1;      /* d[0] = a0*a0 */
        b[0]   = 1;      /* b[0] = a0^3 */
        c[0]   = 1;      /* c[0] = a0^4 */
        max_comp = 0;
    }

    /* enlarge arrays if required */
    if (n > max_comp) {
        size_t needed = (size_t)n + 1;              /* need indices 0..n */
        if (needed > capacity) {
            size_t newcap = capacity;
            while (newcap < needed) newcap <<= 1;
            seq = (seq_t*)realloc(seq, newcap * sizeof(seq_t));
            d   = (seq_t*)realloc(d,   newcap * sizeof(seq_t));
            b   = (seq_t*)realloc(b,   newcap * sizeof(seq_t));
            c   = (seq_t*)realloc(c,   newcap * sizeof(seq_t));
            capacity = newcap;
        }

        /* compute terms from max_comp+1 up to n */
        for (idx_t iter = max_comp + 1; iter <= n; ++iter) {
            seq_t sumProdA = 0;   /* Σ a[i]*a[iter-i],  i=1..iter-1 */
            seq_t sumProdAd = 0;  /* Σ a[i]*d[iter-i],  i=1..iter-1 */

            for (idx_t i = 1; i < iter; ++i) {
                seq_t prodA  = seq[i] * seq[iter - i];
                seq_t prodAd = seq[i] * d[iter - i];
                sumProdA  += prodA;
                sumProdAd += prodAd;
            }

            seq_t Tsum = sumProdA + sumProdAd;
            seq_t delta = (iter == 1) ? 1 : 0;

            /* rhs = 9*c[iter-1] + b[iter-1] - Tsum - delta */
            seq_t rhs = c[iter - 1] * 9 + b[iter - 1] - Tsum - delta;
            seq_t next = rhs / 3;          /* integer division, exact */

            seq[iter] = next;

            /* d[iter] = sum_{i=0}^{iter} seq[i]*seq[iter-i]
                        = 2*seq[iter] + sumProdA */
            d[iter] = next * 2 + sumProdA;

            /* compute b[iter] = Σ seq[i]*d[iter-i] */
            seq_t b_val = 0;
            for (idx_t i = 0; i <= iter; ++i)
                b_val += seq[i] * d[iter - i];
            b[iter] = b_val;

            /* compute c[iter] = Σ seq[i]*b[iter-i] */
            seq_t c_val = 0;
            for (idx_t i = 0; i <= iter; ++i)
                c_val += seq[i] * b[iter - i];
            c[iter] = c_val;
        }
        max_comp = n;
    }

    return seq[n];
}
