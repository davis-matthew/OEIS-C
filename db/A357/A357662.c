/*
 * A357662  Expansion of e.g.f. cosh( (exp(3*x) - 1)/sqrt(3) ).
 * https://oeis.org/A357662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..⌊n/2⌋} 3^{n‑k} * Stirling2(n, 2k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Variables:   seq_t  – type for sequence values (provided in the build)        */
/*              idx_t  – type for indices (provided in the build, uint64_t)   */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for Stirling numbers (1‑D array), powers of 3 and
       the computed a(n) values.  Padding is done in powers of two.       */
    static seq_t *S   = NULL;          /* S[k] holds S(current_n, k)      */
    static seq_t *pow3 = NULL;          /* pow3[i] = 3^i                    */
    static seq_t *a_cache = NULL;       /* cached a(n) values               */
    static idx_t cap   = 0;             /* current capacity of the arrays   */
    static idx_t next  = 0;             /* next index to compute (a_cache)  */

    /* Initialisation: create arrays of initial capacity 16.            */
    if (S == NULL) {
        cap = 16;
        S       = (seq_t *)malloc(cap * sizeof(seq_t));
        pow3    = (seq_t *)malloc(cap * sizeof(seq_t));
        a_cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!S || !pow3 || !a_cache) return (seq_t)0; /* allocation failure */

        /* base values: n = 0 */
        S[0] = (seq_t)1;        /* S(0,0) = 1                         */
        pow3[0] = (seq_t)1;     /* 3^0 = 1                             */
        a_cache[0] = (seq_t)1;  /* a(0) = 1                            */
        next = 1;
    }

    /* Resize arrays if n is beyond current capacity.           */
    if (n >= cap) {
        idx_t old_cap = cap;
        do { cap <<= 1; } while (cap <= n);
        S       = (seq_t *)realloc(S,       cap * sizeof(seq_t));
        pow3    = (seq_t *)realloc(pow3,    cap * sizeof(seq_t));
        a_cache = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
        if (!S || !pow3 || !a_cache) return (seq_t)0; /* allocation failure */

        for (idx_t i = old_cap; i < cap; ++i) {
            S[i]      = (seq_t)0;
            pow3[i]   = (seq_t)0;
            a_cache[i]= (seq_t)0;
        }
    }

    /* Compute all terms up to n that are not yet cached. */
    for (idx_t m = next; m <= n; ++m) {
        if (m > 0) {
            /* Update Stirling numbers S(m, k) using the recurrence
               S(n,k) = k * S(n-1,k) + S(n-1,k-1).                    */
            for (idx_t k = m; k > 0; --k) {
                seq_t old_k   = S[k];          /* old S(m-1,k)          */
                seq_t old_km1 = S[k-1];        /* old S(m-1,k-1)        */
                S[k] = (seq_t)k * old_k + old_km1;
            }
            S[0] = (seq_t)0;                   /* S(m,0) = 0 for m>0   */

            /* Next power of 3. */
            pow3[m] = pow3[m-1] * (seq_t)3;
        }

        /* Evaluate the summation for a(m).                    */
        seq_t sum = (seq_t)0;
        idx_t maxk = m >> 1;                   /* floor(m/2)           */
        for (idx_t k = 0; k <= maxk; ++k) {
            idx_t idx = k << 1;                /* 2*k                  */
            idx_t exp = m - k;                 /* n - k                */
            sum += pow3[exp] * S[idx];
        }
        a_cache[m] = sum;
    }
    next = n + 1;
    return a_cache[n];
}
