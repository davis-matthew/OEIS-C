/*
 * A377682  Expansion of e.g.f. (1 - x * log(1 - x))^2.
 * https://oeis.org/A377682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*n*(n-2)! + 2*n*(n-1)*(n-3)! * H_{n-3}
 *          for n ≥ 3, with H_{k} = 1 + 1/2 + … + 1/k.
 *        a(0)=1, a(1)=0, a(2)=4.
 *
 * This implementation uses factorials and a rational representation of
 * the harmonic number.  All computations are performed with the
 * arbitrary‑precision type seq_t; intermediate values are reduced by
 * common factors with the denominator to keep the products within the
 * range of seq_t when it is a native integer type.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

/* ---------- factorial cache ---------- */
static seq_t *fact_cache = NULL;      /* fact_cache[i] = i!   */
static idx_t  fact_last  = 0;         /* highest n stored     */

/* Ensure the factorial cache holds values up to m. */
static void ensure_factorial(idx_t m)
{
    if (m <= fact_last)
        return;

    size_t new_cap = (size_t)(m + 1);
    if (fact_cache == NULL) {
        fact_cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        fact_cache[0] = (seq_t)1;
        fact_cache[1] = (seq_t)1;
        fact_last   = 1;
    } else if (new_cap > fact_last + 1) {
        fact_cache = (seq_t *)realloc(fact_cache, new_cap * sizeof(seq_t));
    }

    for (idx_t i = fact_last + 1; i <= m; ++i) {
        fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
    }
    fact_last = m;
}

/* Return n! (using cache). */
static seq_t factorial(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    ensure_factorial(n);
    return fact_cache[n];
}

/* ---------- sequence a(n) ---------- */
seq_t a(idx_t n)
{
    /* special cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)4;

    /* term1 = 2*n*(n-2)! */
    seq_t term1 = (seq_t)2 * (seq_t)n;
    term1 *= factorial(n - 2);

    /* compute H_{n-3} as reduced fraction P/Q */
    seq_t P = (seq_t)0;
    seq_t Q = (seq_t)1;
    for (idx_t i = 1; i <= n - 3; ++i) {
        seq_t i_seq = (seq_t)i;
        seq_t newP = P * i_seq + Q;   /* P*i + Q    */
        seq_t newQ = Q * i_seq;       /* Q*i        */
        seq_t g    = gcd(newP, newQ);
        P = newP / g;
        Q = newQ / g;
    }

    /* term2 = 2*n*(n-1)*(n-3)! * P / Q */
    seq_t a_term  = (seq_t)2 * (seq_t)n * (seq_t)(n - 1); /* 2*n*(n-1) */
    seq_t b_term  = factorial(n - 3);                     /* (n-3)!   */

    /* cancel common factors between the factors and Q */
    seq_t g = gcd(a_term, Q);
    a_term /= g;
    Q /= g;

    g = gcd(b_term, Q);
    b_term /= g;
    Q /= g;

    g = gcd(P, Q);
    P /= g;
    Q /= g;

    /* after the reductions Q must be 1 */
    /* compute the remaining product */
    seq_t term2 = a_term;
    term2 *= b_term;  /* a_term * b_term  */
    term2 *= P;       /* multiply by P       */

    /* finally add term1 */
    return term1 + term2;
}
