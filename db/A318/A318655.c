/*
 * A318655  The 2-adic valuation of A318649, the numerators of "Dirichlet
 *          Square Root" of squares.
 * https://oeis.org/A318655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8883, u16 n <= 8883, u32 n <= 8883, u64 n <= 8883,
 *            u128 n <= 8883, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A318655: 2‑adic valuation of the numerators of
    “Dirichlet Square Root” of squares (A318649).
    Recurrence:  g(1)=1,  2·g(n)+∑_{d|n,1<d<n} g(d)g(n/d)=n²  (n≥2),
    let g(n)=Nₙ/2^{Eₙ}  (Nₙ is the integer stored, Eₙ ≥0).
    a(n) = v₂(Nₙ) – Eₙ   (if this value is negative, a(n)=0).  */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Internal arrays: N (low 64 bits of numerator), E (denominator exponent),
   and cache of the returned 2‑adic valuations. */
static uint64_t *g_num     = NULL;   /* low 64 bits of Nₙ              */
static size_t   *g_exp     = NULL;   /* exponent Eₙ                     */
static seq_t   *a_cache   = NULL;   /* cache of a(n)                   */
static size_t   cached    = 0;       /* highest n computed so far      */
static size_t   capacity  = 0;       /* current allocation            */

/* Ensure arrays are allocated and filled up to index N. */
static void
ensure_up_to(idx_t N)
{
    if (N <= cached)
        return;

    /* Allocate/expand arrays to size N+1. */
    if ((size_t)(N + 1) > capacity) {
        size_t new_cap = (size_t)(N + 1);
        g_num = (uint64_t *)realloc(g_num, new_cap * sizeof(uint64_t));
        g_exp = (size_t   *)realloc(g_exp, new_cap * sizeof(size_t));
        a_cache = (seq_t  *)realloc(a_cache, new_cap * sizeof(seq_t));
        capacity = new_cap;
    }

    /* Initialise base case if this is the first call. */
    if (cached == 0) {
        g_num[0] = 0;
        g_exp[0] = 0;
        g_num[1] = 1;          /* g(1) = 1 = 1/2^0            */
        g_exp[1] = 0;
        a_cache[1] = (seq_t)0; /* a(1) = 0                     */
        cached = 1;
    }

    /* Compute g(n) and a(n) for n = cached+1 … N. */
    for (idx_t n = cached + 1; n <= N; ++n) {
        uint64_t sum_num = 0ULL;  /* numerator of the sum (mod 2^64)   */
        size_t   sum_exp = 0;     /* exponent of the denominator      */

        /* Enumerate proper divisors d (2 ≤ d < n).  Only need d ≤ √n. */
        for (idx_t d = 2; d * d <= n; ++d) {
            if (n % d == 0) {
                idx_t d1 = d;
                idx_t d2 = n / d;

                /* Product g(d1)*g(d2).  Keep low 64 bits of the numerator. */
                uint64_t prod_num = g_num[d1] * g_num[d2];
                size_t   prod_exp = g_exp[d1] + g_exp[d2];

                /* Helper to add (prod_num/2^prod_exp) to the running sum. */
                /* Uses the current sum_num/2^sum_exp representation. */
                if (prod_exp > sum_exp) {
                    if (sum_exp < 64)
                        sum_num <<= (prod_exp - sum_exp);
                    else
                        sum_num = 0ULL;
                    sum_exp = prod_exp;
                } else if (prod_exp < sum_exp) {
                    if (prod_exp < 64)
                        prod_num <<= (sum_exp - prod_exp);
                    else
                        prod_num = 0ULL;
                }
                sum_num += prod_num;   /* wraps modulo 2^64 automatically */

                /* If d1 ≠ d2, the pair (d2,d1) contributes an identical
                 * product and must be added a second time.                */
                if (d1 != d2) {
                    uint64_t prod_num2 = prod_num; /* prod_num already fits */
                    size_t   prod_exp2 = prod_exp;
                    if (prod_exp2 > sum_exp) {
                        if (sum_exp < 64)
                            sum_num <<= (prod_exp2 - sum_exp);
                        else
                            sum_num = 0ULL;
                        sum_exp = prod_exp2;
                    } else if (prod_exp2 < sum_exp) {
                        if (prod_exp2 < 64)
                            prod_num2 <<= (sum_exp - prod_exp2);
                        else
                            prod_num2 = 0ULL;
                    }
                    sum_num += prod_num2;
                }
            }
        }

        /* Now compute g(n) = (n² - sum) / 2.  The sum has representation
         * sum_num / 2^sum_exp.  Therefore
         * n² - sum = (n² << sum_exp  -  sum_num) / 2^sum_exp.
         * g(n) = (n² << sum_exp  -  sum_num) / 2^{sum_exp+1}.   */
        uint64_t n_sq = (uint64_t)n * (uint64_t)n;
        uint64_t term_num = 0ULL;          /* n² << sum_exp  (mod 2^64)  */
        if (sum_exp < 64)
            term_num = n_sq << sum_exp;    /* shift is safe because sum_exp < 64 */

        uint64_t diff_num = term_num - sum_num; /* modulo 2^64            */

        size_t exp_n = sum_exp + 1;           /* denominator exponent of g(n) */

        /* Store data for future iterations. */
        g_num[n] = diff_num;   /* low 64 bits of the new numerator       */
        g_exp[n] = exp_n;      /* its denominator exponent               */

        /* Compute a(n) = max(0, v₂(N_n) - E_n).  v₂(N_n) is 
         * the number of trailing zeros of diff_num.                            */
        uint64_t v2_num = (diff_num == 0ULL) ? 64ULL : __builtin_ctzll(diff_num);
        seq_t    a_n = (seq_t) ((v2_num > exp_n) ? (v2_num - exp_n) : 0ULL);

        a_cache[n] = a_n;
    }

    cached = N;
}

seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* undefined for n < 1, return 0 */

    ensure_up_to(n);
    return a_cache[n];
}
