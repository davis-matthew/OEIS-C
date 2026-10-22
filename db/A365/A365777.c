/*
 * A365777  Expansion of e.g.f. (exp(2*x) / (2 - exp(2*x)))^(3/4).
 * https://oeis.org/A365777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A365777: Expansion of e.g.f. (exp(2*x) / (2 - exp(2*x)))^(3/4)
   Formula implemented:  a(n) = Sum_{k=0..n} (-2)^(n-k) *
                            (Product_{j=0..k-1} (4*j+3)) * Stirling2(n,k)
   with a(0) = 1.  Uses memoisation so calls may be in any order.  */
 
#include <stdlib.h>
#include <stdint.h>
 
/* Helper to grow dynamic arrays.  */
static void ensure_cap(size_t *cap, seq_t **arr, size_t need)
{
    if (*cap < need) {
        size_t new_cap = (*cap == 0) ? 1 : (*cap << 1);
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        *arr = tmp;
        *cap = new_cap;
    }
}
 
seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static seq_t *pow2       = NULL;
    static size_t pow2_cap   = 0;
    static seq_t *prod       = NULL;
    static size_t prod_cap   = 0;
    static seq_t *S          = NULL;     /* Stirling numbers of second kind
                                           S(k) holds S(current_n, k) */
    static size_t S_cap      = 0;
    static idx_t  current_n  = 0;
 
    /* Initialise for n==0 */
    if (cache == NULL) {                   /* first call */
        ensure_cap(&cache_cap, &cache, 1);
        ensure_cap(&pow2_cap, &pow2, 1);
        ensure_cap(&prod_cap, &prod, 1);
        ensure_cap(&S_cap, &S, 1);
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        pow2[0]  = (seq_t)1;          /* 2^0 = 1 */
        prod[0]  = (seq_t)1;          /* product of zero terms = 1 */
        S[0]     = (seq_t)1;          /* S(0,0) = 1 */
        current_n = 0;
    }
 
    /* Compute values up to requested n */
    if (n > current_n) {
        for (idx_t i = current_n + 1; i <= n; ++i) {
            /* Resize working arrays */
            ensure_cap(&pow2_cap, &pow2, (size_t)(i + 1));
            ensure_cap(&prod_cap, &prod, (size_t)(i + 1));
            ensure_cap(&S_cap, &S,   (size_t)(i + 1));
            ensure_cap(&cache_cap, &cache, (size_t)(i + 1));
 
            /* Pre‑compute 2^i and product of (4*j+3) up to j=i-1 */
            pow2[i] = pow2[i-1] * (seq_t)2;
            prod[i] = prod[i-1] * (seq_t)(4*(i-1)+3);
 
            /* Update Stirling numbers in place for row i */
            S[i] = (seq_t)0;                          /* S(i,i) starts at 0 */
            for (idx_t k = i; k > 0; --k)
                S[k] = (seq_t)k * S[k] + S[k-1];
            S[0] = (seq_t)0;                          /* S(i,0)=0 for i>0 */
 
            /* Sum over k to obtain a(i) */
            seq_t total = (seq_t)0;
            for (idx_t k = 0; k <= i; ++k) {
                seq_t term = prod[k] * S[k];
                term = term * pow2[i - k];
                if ((i - k) & 1)    /* (-2)^(i-k) is negative when i-k is odd */
                    total -= term;
                else
                    total += term;
            }
            cache[i] = total;
        }
        current_n = n;
    }
 
    return cache[n];
}
