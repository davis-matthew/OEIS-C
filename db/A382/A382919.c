/*
 * A382919  G.f. A(x) satisfies A(x) = 1/( 1 - x*A(x) / (1-x)^3 )^2.
 * https://oeis.org/A382919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) satisfies A(x) = 1/( 1 - x*A(x)/(1-x)^3 )^2. The sequence
 *            is computed by the recurrence obtained from this GF: a[0] = 1;
 *            For n >= 1: E[n] = Σ_{i=0}^{n-1} a[i] * binom(n-1-i+2, 2) E2[n]
 *            = Σ_{i=1}^{n-1} E[i] * E[n-i] a[n] = Σ_{k=0}^{n-1} a[k] * (
 *            2*E[n-k] - E2[n-k] ) All arithmetic is exact fo...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper for multiplication that avoids overflow for integral seq_t types.
   For non‑integral (big‑integer) types it simply uses the * operator. */
#ifdef __cplusplus
#include <type_traits>
template<typename T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
mul_seq(T a, T b)
{
    return (T)((unsigned __int128)a * (unsigned __int128)b);
}

template<typename T>
inline typename std::enable_if<!std::is_integral<T>::value, T>::type
mul_seq(T a, T b)
{
    return a * b;
}
#else
static inline seq_t mul_seq(seq_t a, seq_t b)
{
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
}
#endif

/* Ensure dynamic arrays are large enough. */
static seq_t *A = NULL, *E = NULL, *E2 = NULL;
static size_t cap = 0;
static idx_t last = (idx_t)-1;   /* No values computed yet */

static void ensure_capacity(idx_t n)
{
    if (cap >= (size_t)(n + 1))
        return;
    size_t new_cap = cap ? cap << 1 : 16;
    if (new_cap < (size_t)(n + 1))
        new_cap = n + 1;
    A   = (seq_t*)realloc(A,   new_cap * sizeof(seq_t));
    E   = (seq_t*)realloc(E,   new_cap * sizeof(seq_t));
    E2  = (seq_t*)realloc(E2,  new_cap * sizeof(seq_t));
    /* Initialize new slots */
    for (size_t i = cap; i < new_cap; ++i) {
        A[i]  = 0;
        E[i]  = 0;
        E2[i] = 0;
    }
    cap = new_cap;
    if (last == (idx_t)-1) {   /* first initialization */
        A[0] = 1;  E[0] = 0;  E2[0] = 0;
        last = 0;
    }
}

seq_t a(idx_t n)
{
    ensure_capacity(n);
    if (n <= last)
        return A[n];

    for (idx_t m = last + 1; m <= n; ++m) {
        /* Compute E[m] = Σ_{i=0}^{m-1} a[i] * binom(m-1-i+2,2) */
        seq_t sumE = 0;
        for (idx_t i = 0; i < m; ++i) {
            idx_t j = m - 1 - i;
            seq_t b = ((seq_t)(j + 2) * (j + 1)) / 2;
            seq_t prod = mul_seq(A[i], b);
            sumE += prod;
        }
        E[m] = sumE;

        /* Compute E2[m] = Σ_{i=1}^{m-1} E[i] * E[m-i] */
        seq_t sumE2 = 0;
        for (idx_t i = 1; i < m; ++i) {
            seq_t prod = mul_seq(E[i], E[m - i]);
            sumE2 += prod;
        }
        E2[m] = sumE2;

        /* Compute a[m] = Σ_{k=0}^{m-1} a[k] * (2*E[m-k] - E2[m-k]) */
        seq_t sumA = 0;
        for (idx_t k = 0; k < m; ++k) {
            idx_t s = m - k;
            seq_t diff = mul_seq(E[s] << 1, (seq_t)(1)) - E2[s];
            seq_t prod = mul_seq(A[k], diff);
            sumA += prod;
        }
        A[m] = sumA;
    }

    last = n;
    return A[n];
}
