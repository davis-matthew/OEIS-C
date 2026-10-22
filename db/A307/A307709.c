/*
 * A307709  G.f. A(x) satisfies: A(x) = x*exp(Sum_{n>=1} Sum_{k>=1}
 *          (-1)^(k+1)*n*a(n)*x^(n*k)/k).
 * https://oeis.org/A307709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, * a(n+1) = (1/n) * Σ_{k=1..n} ( Σ_{d|k}
 *            (-1)^(k/d+1) d²*a(d) ) * a(n-k+1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

/* Helper for multiplication with overflow protection on C builds.
 * In C++ (or when seq_t is not an integer), simply use the built‑in *.
 */
static inline seq_t mul_seq(seq_t a, seq_t b)
{
#if !defined(__cplusplus)
    /* 128‑bit arithmetic if available */
#if defined(__SIZEOF_INT128__)
    return (seq_t)((unsigned __int128)a * (unsigned __int128)b);
#else
    return a * b;
#endif
#else
    return a * b;
#endif
}

/* ---------- Memoization storage ---------- */
static seq_t *A = NULL;          /* a(n) values                       */
static seq_t *S = NULL;          /* auxiliary S(k) = Σ_{d|k} (-1)^(k/d+1) d²*a(d) */
static size_t cap = 0;           /* current capacity of A and S       */
static idx_t next_idx = 2;       /* next index to be computed (>=2)   */

/* Ensure arrays can hold indices up to 'need' (inclusive). */
static void ensure_cap(idx_t need)
{
    if ((size_t)need + 1 <= cap) return;
    size_t newcap = cap ? cap * 2 : 128;
    while ((size_t)need + 1 > newcap) newcap <<= 1;
    A = (seq_t*)realloc(A, newcap * sizeof(seq_t));
    S = (seq_t*)realloc(S, newcap * sizeof(seq_t));
    cap = newcap;
}

/* Compute S(k) using currently known a(d).  Result is non‑negative. */
static void compute_S(idx_t k)
{
    signed __int128 sum = 0;
    for (uint32_t d = 1; (uint64_t)d * (uint64_t)d <= k; ++d) {
        if (k % d != 0) continue;
        uint32_t e = (uint32_t)(k / d);
        /* divisor d */
        seq_t term_d = mul_seq((seq_t)d * (seq_t)d, A[d]);
        if (e & 1) sum += (signed __int128)term_d;
        else        sum -= (signed __int128)term_d;
        if (e != d) {
            /* divisor e */
            seq_t term_e = mul_seq((seq_t)e * (seq_t)e, A[e]);
            if (d & 1) sum += (signed __int128)term_e;
            else        sum -= (signed __int128)term_e;
        }
    }
    assert(sum >= 0);                /* by definition */
    S[k] = (seq_t)sum;
}

/* Compute all a(i) up to n (inclusive). */
static void compute_up_to(idx_t n)
{
    if (A == NULL) {               /* initialise a(0)=0, a(1)=1, S(1)=1 */
        ensure_cap(1);
        A[0] = 0;
        A[1] = 1;
        S[1] = 1;
        next_idx = 2;
    }

    while (next_idx <= n) {
        idx_t i = next_idx;        /* we compute a(i)                      */
        idx_t div = i - 1;         /* n in the recurrence                 */

#if !defined(__cplusplus)
        unsigned __int128 sum128 = 0;
        for (idx_t k = 1; k <= div; ++k) {
            seq_t prod = mul_seq(S[k], A[i - k]);
            sum128 += (unsigned __int128)prod;
        }
        A[i] = (seq_t)(sum128 / div);   /* exact division */
#else
        seq_t sum = 0;
        for (idx_t k = 1; k <= div; ++k)
            sum += mul_seq(S[k], A[i - k]);
        A[i] = sum / div;
#endif
        /* compute S(i) for future use */
        compute_S(i);
        next_idx = i + 1;
    }
}

/* Public entry point required by the exercise. */
seq_t a(idx_t n)
{
    if (n < 2) return (n == 0) ? 0 : 1;   /* a(0)=0, a(1)=1 known */
    compute_up_to(n);
    return A[n];
}
