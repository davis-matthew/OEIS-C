/*
 * A375443  Expansion of g.f. A(x) satisfying A(x)^2 = A( x^2/(1-2*x)^3
 *          )/(1-2*x).
 * https://oeis.org/A375443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375443:
   a(0) = 1
   The ordinary generating function A(x) satisfies
         A(x)^2 = A( x^2/(1-2*x)^3 ) / (1-2*x).
   Coefficient recurrence (derived from the functional equation) is:
         a(n) = ( RHS(n) + 2*conv[n-1] - S_n ) / 2    (n ≥ 1)
   where
         RHS(n) = Σ_{m=1}^{⌊n/2⌋} a(m) * binom(m+n-1, n-2m) * 2^(n-2m)
         S_n    = Σ_{i=1}^{n-1} a(i) * a(n-i)
         conv[n] = 2*a(n) + S_n   (i.e. sum_{i=0}^n a(i)*a(n-i))
   The array conv is kept to avoid recomputing the sum S_n in O(n) for each step.

   The implementation below uses memoisation; the sequence values are stored in
   static arrays that grow as needed.  All arithmetic is performed with the
   arbitrary‑precision type seq_t.  Functions provided by the utilities
   (binomial, etc.) are used where appropriate.
*/

#include <stdlib.h>
#include <stdint.h>
#include "../utils/binomial.h"  /* provides seq_t binomial(idx_t n, idx_t k) */

static seq_t *A      = NULL;      /* a(n) values */
static seq_t *conv   = NULL;      /* convolution sums */
static seq_t *P2     = NULL;      /* powers of 2: P2[i] = 2^i */
static size_t  cap   = 0;         /* current allocation capacity */
static size_t  computed = (size_t)-1;  /* largest n already computed; (size_t)-1 → none */

/* Ensure that tables A, conv, P2 are allocated and all values up to n
   have been computed. */
static void ensure_cache(idx_t n)
{
    if (computed != (size_t)-1 && computed >= n) return;

    /* Allocate memory if necessary.  Start with a modest capacity. */
    if (!A)
    {
        cap = 16;
        A   = (seq_t*)malloc(cap * sizeof(seq_t));
        conv = (seq_t*)malloc(cap * sizeof(seq_t));
        P2  = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!A || !conv || !P2) exit(1);

        /* Initial value a(0) = 1.  conv[0] = a0*a0 = 1,  P2[0] = 2^0 = 1. */
        computed = 0;
        A[0]      = (seq_t)1;
        conv[0]   = (seq_t)1;
        P2[0]     = (seq_t)1;
    }

    /* Grow the tables if n is beyond current capacity. */
    if ((size_t)n >= cap)
    {
        size_t newcap = cap;
        while ((size_t)n >= newcap) newcap <<= 1;

        seq_t *newA   = (seq_t*)malloc(newcap * sizeof(seq_t));
        seq_t *newC   = (seq_t*)malloc(newcap * sizeof(seq_t));
        seq_t *newP   = (seq_t*)malloc(newcap * sizeof(seq_t));
        if (!newA || !newC || !newP) exit(1);

        /* Copy already computed entries. */
        for (size_t i = 0; i <= computed; ++i)
        {
            newA[i]   = A[i];
            newC[i]   = conv[i];
            newP[i]   = P2[i];
        }

        free(A);  free(conv);  free(P2);
        A   = newA;
        conv = newC;
        P2  = newP;
        cap  = newcap;
    }

    /* Fill the powers of two that are needed. */
    for (idx_t i = computed + 1; i <= n; ++i)
        P2[i] = P2[i-1] * (seq_t)2;

    /* Compute terms from computed+1 up to n. */
    for (idx_t k = computed + 1; k <= n; ++k)
    {
        /* RHS(k) = Σ_{m=1}^{⌊k/2⌋} a(m) * binom(m+k-1, k-2m) * 2^(k-2m) */
        seq_t RHS = 0;
        idx_t max_m = k >> 1;   /* floor(k/2) */
        for (idx_t m = 1; m <= max_m; ++m)
        {
            seq_t bin  = binomial((idx_t)(m + k - 1),
                                   (idx_t)(k - 2 * m));
            seq_t term = A[(size_t)m] * bin;
            term = term * P2[(size_t)(k - 2 * m)];
            RHS += term;
        }

        /* S_k = Σ_{i=1}^{k-1} a(i) * a(k-i) */
        seq_t S_k = 0;
        for (idx_t i = 1; i <= k - 1; ++i)
            S_k += A[(size_t)i] * A[(size_t)(k - i)];

        /* conv[k-1] = 2*a(k-1) + S_{k-1} is already known. */
        seq_t prev_conv = conv[(size_t)(k - 1)];

        /* a(k) = ( RHS + 2*prev_conv - S_k ) / 2  */
        seq_t tmp = RHS + (seq_t)2 * prev_conv - S_k;
        seq_t a_k = tmp / (seq_t)2;

        A[(size_t)k] = a_k;
        conv[(size_t)k] = (seq_t)2 * a_k + S_k;   /* update convolution sum */
    }

    computed = (size_t)n;
}

/* Public entry point requested by the specification. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return A[(size_t)n];
}
