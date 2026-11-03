/*
 * A272428  Expansion of the continued fraction
 *          x/(1-pi(2)*x/(1-pi(3)*x/(1-pi(4)*x/(1-...)))).
 * https://oeis.org/A272428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   expansion of the continued fraction f(x) =
 *            x/(1-pi(2)*x/(1-pi(3)*x/(1-pi(4)*x/(1-...)))) a(n) is the
 *            coefficient of x^n in f(x), a(0)=0, a(1)=1, a(2)=1, a(3)=3, ...
 *            (OEIS A272428)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 243 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* number of primes <= i, for i up to n+1 */
static void compute_pi_array(idx_t max_n, idx_t **pi_out)
{
    bool *is_prime = (bool *)malloc((max_n + 1) * sizeof(bool));
    for (idx_t i = 0; i <= max_n; ++i) is_prime[i] = true;
    if (max_n >= 0) is_prime[0] = false;
    if (max_n >= 1) is_prime[1] = false;
    for (idx_t i = 2; i * i <= max_n; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= max_n; j += i)
                is_prime[j] = false;
    idx_t *pi = (idx_t *)malloc((max_n + 1) * sizeof(idx_t));
    pi[0] = 0;
    for (idx_t i = 1; i <= max_n; ++i)
        pi[i] = pi[i - 1] + (is_prime[i] ? 1 : 0);
    free(is_prime);
    *pi_out = pi;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    idx_t depth = n + 1;                  /* number of levels needed */
    idx_t *pi = NULL;
    compute_pi_array(depth, &pi);          /* pi[k] for 0..depth */

    /* allocate polynomial arrays of length n+1 (degree <= n) */
    size_t len = (size_t)n + 1;
    seq_t *P = mem_new(seq_t, len);   /* U_k = P/Q */
    seq_t *Q = mem_new(seq_t, len);
    seq_t *Tmp = mem_new(seq_t, len); /* temporary for newP */

    /* base: U_{depth+1} = 1 => P=1, Q=1 */
    P[0] = 1;
    Q[0] = 1;

    for (idx_t k = depth; k >= 2; --k)
    {
        seq_t a_k = (seq_t)pi[k];          /* pi(k) fits in seq_t by construction */

        /* compute newP = P - a_k * x * Q   (shift Q by 1) */
        Tmp[0] = 1;                        /* constant term stays 1 */
        for (size_t i = 1; i < len; ++i)
        {
            seq_t term = Q[i - 1];
            Tmp[i] = P[i] - a_k * term;
        }

        /* Q_new = P_old */
        for (size_t i = 0; i < len; ++i)
            Q[i] = P[i];

        /* P_new = Tmp */
        for (size_t i = 0; i < len; ++i)
            P[i] = Tmp[i];
    }

    /* compute inverse of P modulo x^n (P[0] == 1) */
    size_t inv_len = (size_t)n;                 /* we need up to degree n-1 */
    seq_t *Inv = mem_new(seq_t, inv_len);
    Inv[0] = 1;                                 /* 1/P[0] = 1 */

    for (size_t i = 1; i < inv_len; ++i)
    {
        seq_t sum = 0;
        for (size_t j = 0; j < i; ++j)
            sum += P[i - j] * Inv[j];
        Inv[i] = -sum;                          /* P[0] == 1, divide by 1 */
    }

    /* R(x) = Q / P  modulo x^n  (R[0..n-1]) */
    seq_t *R = mem_new(seq_t, inv_len);
    for (size_t i = 0; i < inv_len; ++i)
    {
        seq_t sum = 0;
        for (size_t j = 0; j <= i; ++j)
            sum += Q[j] * Inv[i - j];
        R[i] = sum;
    }

    /* f(x) = x * R(x)  -> coefficient a(n) = R[n-1] (for n>=1) */
    seq_t result = R[n - 1];

    free(pi);
    mem_free(P);
    mem_free(Q);
    mem_free(Tmp);
    mem_free(Inv);
    mem_free(R);

    return result;
}
