/*
 * A318662  Denominators of the sequence whose Dirichlet convolution with
 *          itself yields A055653, sum of phi(d) over all unitary divisors d
 *          of n.
 * https://oeis.org/A318662
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1538, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A318662 – denominators of the Dirichlet square root of A055653
   f(1) = 1
   f(n) = (1/2) * (A055653(n) - sum_{d|n, 1<d<n} f(d) * f(n/d))       for n > 1
   a(n) = denominator of f(n)
   (All arithmetic is rational, reduced to lowest terms.) */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/gcd.h"

/* ------------------------------------------------------------------ */
/* Rational helpers (num/den stored separately as seq_t)             */
/* ------------------------------------------------------------------ */
static inline void rational_reduce(seq_t *num, seq_t *den)
{
    seq_t g = gcd(*num, *den);
    if (g != (seq_t)1) {
        *num /= g;
        *den /= g;
    }
}

static void rational_mul(seq_t *rnum, seq_t *rden,
                         seq_t n1, seq_t d1, seq_t n2, seq_t d2)
{
    seq_t num = n1 * n2;
    seq_t den = d1 * d2;
    rational_reduce(&num, &den);
    *rnum = num;
    *rden = den;
}

static void rational_add(seq_t *rnum, seq_t *rden,
                         seq_t n1, seq_t d1, seq_t n2, seq_t d2)
{
    seq_t num = n1 * d2 + n2 * d1;
    seq_t den = d1 * d2;
    rational_reduce(&num, &den);
    *rnum = num;
    *rden = den;
}

static void rational_sub(seq_t *rnum, seq_t *rden,
                         seq_t n1, seq_t d1, seq_t n2, seq_t d2)
{
    /* Assumes n1/d1 >= n2/d2 (holds in this algorithm) */
    seq_t num = n1 * d2 - n2 * d1;
    seq_t den = d1 * d2;
    rational_reduce(&num, &den);
    *rnum = num;
    *rden = den;
}

/* ------------------------------------------------------------------ */
/* Smallest prime factor sieve (malloced once per program)            */
/* ------------------------------------------------------------------ */
static uint64_t *g_spf = NULL;
static size_t g_spf_cap = 0;

static void ensure_spf(idx_t n)
{
    if (n < 2 || (g_spf_cap > 0 && g_spf_cap > n))
        return;

    size_t new_cap = (g_spf_cap > 0) ? g_spf_cap * 2 : 2;
    while (new_cap <= n)
        new_cap <<= 1;

    uint64_t *new_spf = (uint64_t *)realloc(g_spf, new_cap * sizeof(uint64_t));
    if (!new_spf)
        exit(1);

    if (g_spf == NULL) {
        for (size_t i = 0; i < new_cap; ++i)
            new_spf[i] = i;
    } else {
        for (size_t i = g_spf_cap; i < new_cap; ++i)
            new_spf[i] = i;
    }

    /* Sieve */
    size_t limit = new_cap;
    for (size_t i = 2; i * i < limit; ++i) {
        if (new_spf[i] == i) {
            for (size_t j = i * i; j < limit; j += i)
                if (new_spf[j] == j)
                    new_spf[j] = i;
        }
    }

    g_spf = new_spf;
    g_spf_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Factorisation helper (uses g_spf)                                 */
/* ------------------------------------------------------------------ */
typedef struct { uint64_t p; int e; } prime_exp;

static void factorize_u64(uint64_t n, prime_exp *buf, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint64_t p = g_spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        buf[(*cnt)++] = (prime_exp){p, e};
    }
}

/* ------------------------------------------------------------------ */
/* Compute A055653(n) – sum of phi over unitary divisors              */
/* ------------------------------------------------------------------ */
static seq_t compute_A055653(idx_t n)
{
    prime_exp pf[64];
    size_t pcnt;
    factorize_u64((uint64_t)n, pf, &pcnt);

    seq_t res = (seq_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)pf[i].p;
        int e = pf[i].e;
        seq_t p_pow = (seq_t)1;
        for (int k = 1; k < e; ++k)
            p_pow *= p;                     /* p^(e-1) */
        seq_t term = (seq_t)1 + p_pow * (p - (seq_t)1);   /* 1 + p^(e-1)*(p-1) */
        res *= term;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Storage for f(n) as fractions: g_num[n] / g_den[n]                */
/* ------------------------------------------------------------------ */
static seq_t *g_num = NULL, *g_den = NULL;
static size_t g_cap = 0;

static void ensure_cap(idx_t n)
{
    if (g_cap > 0 && g_cap > n)
        return;
    size_t new_cap = (g_cap > 0) ? g_cap * 2 : 16;
    while (new_cap <= n)
        new_cap <<= 1;
    seq_t *new_num = (seq_t *)realloc(g_num, new_cap * sizeof(seq_t));
    seq_t *new_den = (seq_t *)realloc(g_den, new_cap * sizeof(seq_t));
    if (!new_num || !new_den)
        exit(1);
    for (size_t i = g_cap; i < new_cap; ++i) {
        new_num[i] = (seq_t)0;
        new_den[i] = (seq_t)1;
    }
    g_num = new_num;
    g_den = new_den;
    g_cap = new_cap;
}

/* ------------------------------------------------------------------ */
/* Recursive computation of f(n)                                      */
/* ------------------------------------------------------------------ */
static void compute_f(idx_t n)
{
    if (n == 1) {
        g_num[1] = (seq_t)1;
        g_den[1] = (seq_t)1;
        return;
    }
    if (g_den[n] != (seq_t)1)   /* already computed */
        return;
    ensure_cap(n);

    /* Sum of phi over unitary divisors */
    seq_t sumphi = compute_A055653(n);

    /* Generate all divisors of n */
    prime_exp pf[64];
    size_t pcnt;
    factorize_u64((uint64_t)n, pf, &pcnt);
    idx_t divisors[256];
    size_t dsize = 1;
    divisors[0] = (idx_t)1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pf[i].p;
        int e = pf[i].e;
        size_t old = dsize;
        idx_t p_pow = 1;
        for (int k = 1; k <= e; ++k) {
            p_pow *= (idx_t)p;
            for (size_t j = 0; j < old; ++j)
                divisors[dsize++] = divisors[j] * p_pow;
        }
    }

    /* Sum over proper divisors (excluding 1 and n) */
    seq_t sum_num = (seq_t)0;
    seq_t sum_den = (seq_t)1;
    for (size_t i = 0; i < dsize; ++i) {
        idx_t d = divisors[i];
        if (d <= 1 || d >= n)
            continue;
        idx_t nd = n / d;
        compute_f(d);
        compute_f(nd);
        seq_t n1 = g_num[d], d1 = g_den[d];
        seq_t n2 = g_num[nd], d2 = g_den[nd];
        seq_t prod_num, prod_den;
        rational_mul(&prod_num, &prod_den, n1, d1, n2, d2);
        seq_t tmp_num, tmp_den;
        rational_add(&tmp_num, &tmp_den, sum_num, sum_den, prod_num, prod_den);
        sum_num = tmp_num;
        sum_den = tmp_den;
    }

    /* diff = sumphi - sumprod */
    seq_t diff_num, diff_den;
    rational_sub(&diff_num, &diff_den, sumphi, (seq_t)1, sum_num, sum_den);

    /* f(n) = diff / 2 */
    seq_t fn_num = diff_num;
    seq_t fn_den = diff_den * (seq_t)2;
    rational_reduce(&fn_num, &fn_den);

    g_num[n] = fn_num;
    g_den[n] = fn_den;
}

/* ------------------------------------------------------------------ */
/* Public API: returns the denominator a(n)                           */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* out of range, not used */

    ensure_spf(n);
    ensure_cap(n);
    compute_f(n);
    return g_den[n];
}
