/*
 * A363480  G.f. satisfies A(x) = exp( Sum_{k>=1} A(2*x^k)^2 * x^k/k ).
 * https://oeis.org/A363480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A363480
   G.f. satisfies A(x) = exp( Σ_{k≥1} A(2*x^k)^2 * x^k / k )
   a(0)=1, a(1)=1, a(2)=5, a(3)=49, a(4)=923, a(5)=32603, … */

/* This implementation computes the sequence by using the
   recurrence derived from the exponential generating‑function relation.
   Let B_n = 2^{n-1} * Σ_{i=0}^{n-1} a[i]·a[n-1-i].
   Let P_m = Σ_{t|m} t·B_t.
   Then for n≥1
          a[n] = (1/n) · Σ_{m=1}^{n} a[n-m] · P_m .
   The arrays a, B, P and pow2 are kept in static memory and
   extended on demand, allowing calls in arbitrary order. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static arrays that keep computed values between calls */
    static seq_t *aarr  = NULL;
    static seq_t *Barr  = NULL;
    static seq_t *Par   = NULL;
    static seq_t *pow2  = NULL;
    static size_t cap = 0;      /* allocated length of the arrays       */
    static idx_t  computed = 0; /* next index to compute (a[computed]) */

    /* helper to ensure capacity is at least m */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap? (cap << 1) : 16;
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;

        aarr  = (seq_t *)realloc(aarr,  newcap * sizeof(seq_t));
        Barr  = (seq_t *)realloc(Barr,  newcap * sizeof(seq_t));
        Par   = (seq_t *)realloc(Par,   newcap * sizeof(seq_t));
        pow2  = (seq_t *)realloc(pow2,  newcap * sizeof(seq_t));
        if (!aarr || !Barr || !Par || !pow2) return (seq_t)0;

        for (size_t i = cap; i < newcap; ++i) {
            aarr[i] = (seq_t)0;
            Barr[i] = (seq_t)0;
            Par[i]  = (seq_t)0;
            pow2[i] = (seq_t)0;
        }
        cap = newcap;
    }

    /* initialise the zeroth element on first use */
    if (computed == 0) {
        aarr[0] = (seq_t)1;
        pow2[0] = (seq_t)1;
        /* B_0 and P_0 are unused but keep them zero for safety */
        Barr[0] = (seq_t)0;
        Par[0]  = (seq_t)0;
        computed = 1;                /* next to compute is a[1]           */
    }

    /* compute all missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        /* compute B_i = 2^{i-1} * Σ_{j=0}^{i-1} a_j·a_{i-1-j} */
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j < i; ++j) {
            sum += aarr[j] * aarr[i - 1 - j];
        }
        Barr[i] = pow2[i - 1] * sum;

        /* update pow2[i] = 2^i */
        pow2[i] = pow2[i - 1] * (seq_t)2;

        /* add contribution of B_i to all P_m with m multiple of i */
        seq_t weight = (seq_t)i * Barr[i];
        for (idx_t m = i; m < cap; m += i) {
            Par[m] += weight;
        }

        /* compute a[i] = (1/i) * Σ_{m=1}^{i} a[i-m]·P_m */
        seq_t S = (seq_t)0;
        for (idx_t m = 1; m <= i; ++m) {
            S += aarr[i - m] * Par[m];
        }
        aarr[i] = S / (seq_t)i;
    }

    /* update the marker of the next index to compute */
    computed = n + 1;
    return aarr[n];
}
