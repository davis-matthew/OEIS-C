/*
 * A346895  Expansion of e.g.f. 1 / (1 - (exp(x) - 1)^4 / 4!).
 * https://oeis.org/A346895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A346895
   Recurrence: a(0)=1; for n≥1,
   a(n) = Σ_{k=1..n} binomial(n,k) * S(k,4) * a(n-k),
   where S(k,4) is the Stirling number of the second kind.  A closed form for
   S(k,4) when k≥4 is
   S(k,4) = (4^k – 4·3^k + 6·2^k – 4)/24.  The implementation uses this
   closed form and an iterative computation of binomial(n,k), so no heavy
   pre‑computation or recursion is required.  Values are cached so that
   calls in any order remain efficient.  Complexity per call is O(n).  */

#include <stdlib.h>
#include <stdint.h>

/* function prototype from the build environment */
seq_t *seq_realloc(seq_t *old, size_t old_cap, size_t new_cap);

/* sequence computation */
seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *val = NULL;
    static size_t cap = 0;        /* current capacity of val[] */
    static size_t computed = 0;   /* number of values already computed (next to compute) */

    /* ensure array is large enough for index n */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_val = (seq_t *)realloc(val, new_cap * sizeof(seq_t));
        if (!new_val) return 0;   /* allocation failure – return 0 as a safety */
        val = new_val;
        cap = new_cap;
    }

    /* initialise a(0) if not done yet */
    if (computed == 0) {
        val[0] = 1;
        computed = 1;
    }

    /* compute missing values up to n */
    while (computed <= n) {
        idx_t m = computed;     /* current index to compute */

        /* initialise a[m] to 0 before summing */
        val[m] = 0;

        /* iterative computation of binomial coefficients and powers for S(k,4) */
        seq_t binom = 1;   /* binomial(m,0) = 1 */
        seq_t p2 = 1;      /* 2^0 */
        seq_t p3 = 1;      /* 3^0 */
        seq_t p4 = 1;      /* 4^0 */

        for (idx_t k = 1; k <= m; ++k) {
            /* binomial(m,k) = binomial(m,k-1) * (m-k+1) / k */
            binom = (binom * (m - k + 1)) / k;

            /* update powers */
            p2 *= 2;
            p3 *= 3;
            p4 *= 4;

            /* Stirling number S(k,4) = (p4 - 4*p3 + 6*p2 - 4)/24 */
            seq_t numerator = p4 - 4 * p3 + 6 * p2 - 4;
            seq_t s4 = numerator / 24; /* exact division */

            /* accumulate term binom * s4 * a[m-k] */
            seq_t term = binom * s4 * val[m - k];
            val[m] += term;
        }

        computed++;   /* next value will be computed on subsequent calls */
    }

    return val[n];
}
