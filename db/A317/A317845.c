/*
 * A317845  Numerators of rational valued sequence whose Dirichlet convolution
 *          with itself yields sequence A001065 (sum of proper divisors) +
 *          A063524 (1, 0, 0, 0, ...).
 * https://oeis.org/A317845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of f(n), where * f(1) = 1, * f(n) = 1/2 *
 *            (A001065(n) - Σ_{d|n, 1<d<n} f(d) * f(n/d)) for n > 1 *
 *            (A001065(n) is the sum of proper divisors of n.)
 * Range:     i8 n <= 11, i16 n <= 127, i32 n <= 4095, i64 n <= 9625,
 *            i128 n <= 9625, big any n
 * Helpers:   pow2int, sum_proper_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/pow2int.h"
#include "../utils/sum_proper_divisors.h"

/* local helpers ---------------------------------------------------------- */

/* cache for f(n): numerator and exponent of denominator 2^exp */
static seq_t *cache_num = NULL;
static int   *cache_exp = NULL;
static char  *cache_known = NULL;
static size_t cache_size = 0;

static void ensure_cache(idx_t n)
{
    /* enlarge cache to hold indices up to n (inclusive) */
    if (n + 1 > cache_size) {
        size_t new_size = n + 1;
        if (new_size < 16) new_size = 16;
        if (new_size < cache_size * 2) new_size = cache_size * 2;
        cache_num   = (seq_t *)realloc(cache_num,   new_size * sizeof(seq_t));
        cache_exp   = (int *)   realloc(cache_exp,   new_size * sizeof(int));
        cache_known = (char *)  realloc(cache_known, new_size * sizeof(char));
        for (size_t i = cache_size; i < new_size; ++i)
            cache_known[i] = 0;
        cache_size = new_size;
    }
}

/* sum of proper divisors (A001065) --------------------------------------- */

/* compute f(n) and return its numerator (after reduction) ---------------- */

static seq_t compute_f(idx_t n)
{
    ensure_cache(n);
    if (cache_known[n])               /* already computed */
        return cache_num[n];

    /* base case */
    if (n == 1) {
        cache_num[1] = (seq_t)1;
        cache_exp[1] = 0;
        cache_known[1] = 1;
        return cache_num[1];
    }

    /* collect terms f(d)*f(n/d) for all proper divisors d>1, d<n */
    struct Term { seq_t num; int exp; } terms[256];
    int term_cnt = 0;
    int Dmax = 0;

    for (idx_t d = 2; d <= n / d; ++d) {
        if (n % d != 0) continue;
        idx_t d1 = d;
        idx_t d2 = n / d;

        compute_f(d1);
        compute_f(d2);
        seq_t num = cache_num[d1] * cache_num[d2];
        int   exp = cache_exp[d1] + cache_exp[d2];
        if (exp > Dmax) Dmax = exp;
        terms[term_cnt++] = (struct Term){num, exp};

        if (d1 != d2) {            /* the symmetric divisor */
            /* product is the same, push again */
            terms[term_cnt++] = (struct Term){num, exp};
        }
    }

    /* sum with common denominator 2^Dmax */
    seq_t sum_numer = 0;
    for (int i = 0; i < term_cnt; ++i) {
        int shift = Dmax - terms[i].exp;
        sum_numer += terms[i].num * pow2int(shift);
    }

    /* compute f(n): (h * 2^Dmax - sum_numer) / 2^{Dmax+1} */
    seq_t h = sum_proper_divisors(n);

    seq_t numerator = h * pow2int(Dmax) - sum_numer;
    int   den_exp   = Dmax + 1;

    /* reduce numerator / 2^den_exp */
    while (den_exp > 0 && (numerator % 2 == 0)) {
        numerator /= 2;
        den_exp--;
    }

    cache_num[n]   = numerator;
    cache_exp[n]   = den_exp;
    cache_known[n] = 1;
    return numerator;
}

/* public entry ------------------------------------------------------------ */

seq_t a(idx_t n)
{
    return compute_f(n);
}
