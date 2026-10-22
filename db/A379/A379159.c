/*
 * A379159  G.f. A(x) satisfies A(x) = (1 + x^3) * (1 + x*A(x)^3).
 * https://oeis.org/A379159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A379159
 *  The sequence is defined by the generating function
 *  A(x) = (1 + x^3) * (1 + x * A(x)^3).
 *  From this we obtain the recurrence
 *      a(0) = 1,  a(3) = 1
 *      a(n) = δ_{n,0} + δ_{n,3} + Σ_{i=0}^{n-1} a(i) * c(n-1-i)
 *             + Σ_{i=0}^{n-4} a(i) * c(n-4-i)   (n ≥ 0),
 *  where c(m) is the coefficient of x^m in A(x)^3, i.e.
 *      c(m) = Σ_{i=0}^{m} a(i) * b(m-i),   b(k)=Σ_{j=0}^{k} a(j)a(k-j).
 *  The implementation below keeps the values a(k) and the two‐fold
 *  convolution b(k) in cached static arrays so that any order of calls
 *  is handled in O(n) time per new index.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* -------------------------------------------------------------
     * static cache
     * ------------------------------------------------------------- */
    static idx_t   max_n   = 0;      // last computed index
    static seq_t *values  = NULL;   // a(k) values
    static seq_t *bconv   = NULL;   // a*a convolution (b(k))
    static size_t  val_cap = 0;     // allocated capacity for values
    static size_t  conv_cap= 0;     // allocated capacity for bconv

    /* -------------------------------------------------------------
     * initialise cache for the first call (including the case a(0))
     * ------------------------------------------------------------- */
    if (max_n == 0 && values == NULL) {
        /* allocate space for the zeroth element */
        val_cap  = 1;
        conv_cap = 1;
        values   = (seq_t *)malloc(val_cap * sizeof(seq_t));
        bconv    = (seq_t *)malloc(conv_cap * sizeof(seq_t));
        if (!values || !bconv) {
            /* out of memory – return 0 as a safe default */
            return (seq_t)0;
        }
        values[0] = (seq_t)1;  /* a(0) = 1 */
        bconv[0]  = (seq_t)1;  /* b(0) = a0*a0 = 1 */
    }

    /* -------------------------------------------------------------
     * extend the cache up to index n
     * ------------------------------------------------------------- */
    if (n > max_n) {
        /* enlarge arrays if necessary */
        size_t new_val_cap = (size_t)(n + 1);
        if (new_val_cap > val_cap) {
            seq_t *tmp = (seq_t *)realloc(values, new_val_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;           /* allocation failed */
            /* zero the newly allocated slots */
            for (size_t i = val_cap; i < new_val_cap; ++i)
                tmp[i] = (seq_t)0;
            values   = tmp;
            val_cap  = new_val_cap;
        }

        size_t new_conv_cap = (size_t)(n + 1);
        if (new_conv_cap > conv_cap) {
            seq_t *tmp = (seq_t *)realloc(bconv, new_conv_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            for (size_t i = conv_cap; i < new_conv_cap; ++i)
                tmp[i] = (seq_t)0;
            bconv    = tmp;
            conv_cap = new_conv_cap;
        }

        /* compute terms from max_n+1 up to n */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t term = (seq_t)0;

            /* δ_{k,0} and δ_{k,3} */
            if (k == 0) term += (seq_t)1;
            if (k == 3) term += (seq_t)1;

            /* Σ_{i=0}^{k-1} a(i) * d(k-1-i)   (d = bconv) */
            if (k >= 1) {
                seq_t sum = (seq_t)0;
                for (idx_t i = 0; i <= k - 1; ++i)
                    sum += values[(size_t)i] * bconv[(size_t)((k - 1) - i)];
                term += sum;
            }

            /* Σ_{i=0}^{k-4} a(i) * d(k-4-i) */
            if (k >= 4) {
                seq_t sum = (seq_t)0;
                for (idx_t i = 0; i <= k - 4; ++i)
                    sum += values[(size_t)i] * bconv[(size_t)((k - 4) - i)];
                term += sum;
            }

            /* store the new value */
            values[(size_t)k] = term;

            /* update the convolution d(k) = Σ_{i=0}^k a(i)*a(k-i) */
            seq_t d_k = (seq_t)0;
            for (idx_t i = 0; i <= k; ++i)
                d_k += values[(size_t)i] * values[(size_t)(k - i)];
            bconv[(size_t)k] = d_k;
        }

        max_n = n;
    }

    /* -------------------------------------------------------------
     * return cached result
     * ------------------------------------------------------------- */
    return values[(size_t)n];
}
