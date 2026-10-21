/*
 * A243585  Expansion of x*log'(C(C(x)-1)-1), C(x) = (1-sqrt(1-4*x))/(2*x).
 * https://oeis.org/A243585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} binomial(2*k,k) * binomial(2*n,n-k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Cache for central binomial coefficients C_k = binomial(2*k,k). */
    static seq_t *Ccache = NULL;
    static size_t Ccap   = 0;
    static size_t Csize  = 0;

    /* Initialise cache on first call. */
    if (Csize == 0) {
        Ccap   = 1;
        Ccache = (seq_t *)malloc(Ccap * sizeof(seq_t));
        if (!Ccache) return (seq_t)0;
        Ccache[0] = (seq_t)1;          /* C_0 = 1 */
        Csize   = 1;
    }

    /* Extend cache up to index n. */
    while (Csize <= n) {
        if (Csize >= Ccap) {                     /* grow cache if needed */
            size_t new_cap = Ccap ? Ccap * 2 : 1;
            while (new_cap <= Csize) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(Ccache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            Ccache = tmp;
            Ccap   = new_cap;
        }
        seq_t prev  = Ccache[Csize-1];
        seq_t mult  = (seq_t)(2 * (2 * (Csize-1) + 1));   /* 2*(2k+1) */
        seq_t next  = seq_mul(prev, mult) / (seq_t)Csize; /* C_{k+1} */
        Ccache[Csize] = next;
        ++Csize;
    }

    /* Compute a(n) = Σ_{k=0..n} C_k * binom(2n, n-k). */
    seq_t sum   = (seq_t)0;
    seq_t binom = (seq_t)1;                  /* binom(2n,0) */

    for (idx_t r = 0; r <= n; ++r) {
        seq_t prod = seq_mul(Ccache[n - r], binom);
        sum += prod;
        if (r < n) {
            seq_t tnum = seq_mul(binom, (seq_t)(2 * n - r));
            binom = tnum / (seq_t)(r + 1);
        }
    }
    return sum;
}
