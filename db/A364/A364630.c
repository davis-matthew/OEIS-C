/*
 * A364630  G.f. satisfies A(x) = (1+x*A(x)^4)/(1-x)^2.
 * https://oeis.org/A364630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A364630
 * Recurrence derived from the generating function:
 *   a(0) = 1
 *   a(n) = 2*a(n-1) – a(n-2) + c(n-1)   (n ≥ 1)
 * where c(m) = Σ_{i+j+k+l=m} a(i)*a(j)*a(k)*a(l)
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* multiply two seq_t values using unsigned __int128,
 * to avoid overflow when seq_t is 64‑bit.
 */
static inline seq_t mul_safe(seq_t a, seq_t b)
{
    return (seq_t)((unsigned __int128)a * b);
}

seq_t a(idx_t n)
{
    /* static state for incremental computation */
    static seq_t *A      = NULL;   /* a[n]                */
    static seq_t *P2     = NULL;   /* 2‑fold convolution  */
    static seq_t *P3     = NULL;   /* 3‑fold convolution  */
    static size_t capA   = 0;
    static size_t capP2  = 0;
    static size_t capP3  = 0;
    static idx_t   max_n  = 0;     /* highest n computed  */

    /* initialise for n = 0 */
    if (A == NULL) {
        A   = (seq_t *)malloc(sizeof(seq_t));
        P2  = (seq_t *)malloc(sizeof(seq_t));
        P3  = (seq_t *)malloc(sizeof(seq_t));
        if (!A || !P2 || !P3) exit(EXIT_FAILURE);
        capA  = capP2 = capP3 = 1;

        A[0] = 1;                       /* a(0) */
        P2[0] = mul_safe(A[0], A[0]);   /* a0^2 */
        P3[0] = mul_safe(mul_safe(A[0], A[0]), A[0]); /* a0^3 */
        max_n = 0;
    }

    /* grow arrays as required to compute a(n) */
    while (max_n < n) {
        idx_t next = max_n + 1;                 /* compute a(next) */

        /* enlarge A if needed */
        if (capA <= next) {
            size_t newcap = capA ? capA * 2 : 1;
            while (newcap <= next) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(A, sizeof(seq_t) * newcap);
            if (!tmp) exit(EXIT_FAILURE);
            if (newcap > capA)
                memset(tmp + capA, 0, sizeof(seq_t) * (newcap - capA));
            A = tmp; capA = newcap;
        }

        /* enlarge P2 */
        if (capP2 <= 2 * next) {
            size_t desired = 2 * next + 1;
            size_t newcap = capP2 ? capP2 * 2 : 1;
            while (newcap < desired) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(P2, sizeof(seq_t) * newcap);
            if (!tmp) exit(EXIT_FAILURE);
            if (newcap > capP2)
                memset(tmp + capP2, 0, sizeof(seq_t) * (newcap - capP2));
            P2 = tmp; capP2 = newcap;
        }

        /* enlarge P3 */
        if (capP3 <= 3 * next) {
            size_t desired = 3 * next + 1;
            size_t newcap = capP3 ? capP3 * 2 : 1;
            while (newcap < desired) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(P3, sizeof(seq_t) * newcap);
            if (!tmp) exit(EXIT_FAILURE);
            if (newcap > capP3)
                memset(tmp + capP3, 0, sizeof(seq_t) * (newcap - capP3));
            P3 = tmp; capP3 = newcap;
        }

        /* compute c(next-1) using existing P3 */
        idx_t m = next - 1;
        seq_t sum = 0;
        for (idx_t i = 0; i <= m; ++i) {
            seq_t prod = mul_safe(A[i], P3[m - i]);
            sum += prod;
        }

        seq_t a_prev  = A[max_n];
        seq_t a_prev2 = (max_n >= 1) ? A[max_n - 1] : (seq_t)0;

        seq_t a_next = a_prev + a_prev;     /* 2*a_prev */
        a_next -= a_prev2;
        a_next += sum;

        A[next] = a_next;                   /* store new term */

        /* update 3‑fold convolution P3 for indices up to 3*next */
        for (idx_t t = next; t <= 3 * next; ++t) {
            seq_t val = 0;

            /* one a_next in the product a_next * P2[t-next] */
            if (t >= next) {
                idx_t idx1 = t - next;
                if (idx1 <= 2 * (next - 1)) {
                    seq_t tmp = mul_safe(a_next, P2[idx1]);   /* a_n * P2 */
                    tmp = mul_safe(tmp, (seq_t)3);            /* *3 for permutations */
                    val += tmp;
                }
            }

            /* two a_next: a_next^2 * A[t-2*next] */
            if (t >= 2 * next) {
                idx_t idx2 = t - 2 * next;
                if (idx2 <= max_n) {
                    seq_t tmp = mul_safe(a_next, a_next);    /* a_n^2 */
                    tmp = mul_safe(tmp, A[idx2]);            /* * A[idx2] */
                    tmp = mul_safe(tmp, (seq_t)3);            /* *3 for permutations */
                    val += tmp;
                }
            }

            /* three a_next: a_next^3 (only when t==3*next) */
            if (t == 3 * next) {
                seq_t tmp = mul_safe(a_next, a_next);
                tmp = mul_safe(tmp, a_next);
                val += tmp;
            }

            P3[t] += val;
        }

        /* update 2‑fold convolution P2 for indices up to 2*next */
        for (idx_t s = next; s <= 2 * next; ++s) {
            idx_t other = s - next;
            seq_t contrib = mul_safe(a_next, A[other]);   /* a_n * A[other] */
            if (s == 2 * next) {
                P2[s] += contrib;                    /* (n,n) pair */
            } else {
                P2[s] += contrib + contrib;          /* (n,other) and (other,n) */
            }
        }

        max_n = next;   /* advance computed range */
    }

    return A[n];
}
