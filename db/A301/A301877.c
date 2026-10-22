/*
 * A301877  Group the natural numbers into groups
 *          (1),(2),(3),(4),(5,6),(7,8,9),... so that the n-th group contains
 *          N(n) terms, where N(n) is the Narayana's cows sequence (A000930).
 *          Sequence contains the sum of the terms in the n-th group.
 * https://oeis.org/A301877
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computation of OEIS sequence A301877.
 *
 * Let N(n) be the Narayana's cows sequence starting with
 *    N(1)=N(2)=N(3)=N(4)=1
 *    N(k)=N(k-1)+N(k-3)   for k≥5
 *
 * Let P(n) = ∑_{i=1}^{n} N(i)  (with P(0)=0).
 *
 * The nth term is the sum of the consecutive integers belonging to
 * group n, i.e.
 *     a(n) = N(n) * (1 + 2*P(n-1) + N(n)) / 2
 *        = N(n) * (N(n)+1)/2 + N(n) * P(n-1)
 *
 * The implementation memoizes N and P up to the largest requested n.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *N = NULL;      /* N[0] unused, N[1]..N[n] filled */
    static seq_t *P = NULL;      /* P[0] = 0, P[i] = sum_{k=1}^{i} N(k) */
    static idx_t cap = 0;        /* allocated array size (≥ n+1) */
    static idx_t len = 0;        /* highest index computed so far */

    if (cap == 0) { /* first call: allocate minimal capacity */
        cap = 8;
        N = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        P = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        if (!N || !P) abort();
        P[0] = 0;
    }

    /* Extend arrays if necessary */
    while (cap <= n) {
        idx_t new_cap = cap << 1;
        if (new_cap < cap) new_cap = (idx_t)-1; /* overflow guard */
        N = (seq_t *)realloc(N, new_cap * sizeof(seq_t));
        P = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
        if (!N || !P) abort();
        cap = new_cap;
    }

    /* Compute N and P up to n if not already */
    for (idx_t i = len + 1; i <= n; ++i) {
        if (i <= 4) {
            N[i] = (seq_t)1;
        } else {
            N[i] = N[i - 1] + N[i - 3];
        }
        P[i] = P[i - 1] + N[i];
    }
    len = n;

    seq_t Nn = N[n];
    seq_t sum_prev = P[n - 1];

    /* Compute N(n)*(N(n)+1)/2 without overflow */
    seq_t half_part;
    if ((Nn & (seq_t)1) == 0) {
        half_part = (Nn / (seq_t)2) * (Nn + (seq_t)1);
    } else {
        half_part = Nn * ((Nn + (seq_t)1) / (seq_t)2);
    }

    /* a(n) = half_part + N(n) * sum_prev */
    return half_part + Nn * sum_prev;
}
