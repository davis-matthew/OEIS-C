/*
 * A277440  Denominators of a sequence defined by a modified recurrence for
 *          the exponential of the von Mangoldt function.
 * https://oeis.org/A277440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of T(n,1) where T(n,1) is defined recursively
 *            by: T(1,1)=1. T(n,k)=1 if k>1 and n mod k != 0, else T(n/k,1).
 *            If k=1 then T(n,1)= n / (∏_{i=1}^{n-1} T(n+1,i+1)) /
 *            (∏_{i=1}^{n-1} T(n-1,i+1)). a(n)=denominator of T(n,1) in lowest
 *            terms
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 40, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* External type definitions from the build */
extern seq_t gcd(seq_t a, seq_t b);

static seq_t *num_table = NULL;   /* numerator of T(i,1) */
static seq_t *den_table = NULL;   /* denominator of T(i,1) */
static size_t allocated = 0;

/* Ensure tables hold at least indices up to idx (inclusive) */
static void need_size(idx_t idx)
{
    if (idx + 1 <= allocated) return;
    size_t new_alloc = (idx + 1) * 2;
    seq_t *new_num = (seq_t *)realloc(num_table, new_alloc * sizeof(seq_t));
    seq_t *new_den = (seq_t *)realloc(den_table, new_alloc * sizeof(seq_t));
    if (!new_num || !new_den) {
        /* Allocation failed – abort */
        exit(EXIT_FAILURE);
    }
    /* Initialize new entries */
    for (size_t i = allocated; i < new_alloc; ++i) {
        new_num[i] = 0;
        new_den[i] = 0;
    }
    num_table = new_num;
    den_table = new_den;
    allocated = new_alloc;
}

/* Multiply two rational factors in situ with reduction to avoid overflow where possible. */
static void mul_reduce(seq_t *prod_num, seq_t *prod_den,
                       seq_t factor_num, seq_t factor_den)
{
    /* Cancel cross factors before multiplication */
    seq_t g1 = gcd(*prod_num, factor_den);
    seq_t g2 = gcd(*prod_den, factor_num);

    *prod_num /= g1;
    *prod_den /= g2;
    factor_den /= g1;
    factor_num /= g2;

    /* Multiply */
    seq_t n = (*prod_num) * factor_num;
    seq_t d = (*prod_den) * factor_den;

    /* Final reduction */
    seq_t g3 = gcd(n, d);
    *prod_num = n / g3;
    *prod_den = d / g3;
}

/* Recursively compute T(n,1) numerator and denominator.
   Assumes n >= 1. */
static void compute_T(idx_t n)
{
    need_size(n);
    if (num_table[n] != 0 || den_table[n] != 0) return;  /* already computed */

    if (n == 1) {
        num_table[1] = 1;
        den_table[1] = 1;
        return;
    }

    seq_t prodN1 = 1, prodD1 = 1;
    idx_t m1 = n + 1;

    /* Divisors of m1 excluding 1 and m1 */
    for (idx_t d = 2; d * d <= m1; ++d) {
        if (m1 % d != 0) continue;
        /* divisor d */
        idx_t t1 = m1 / d;
        compute_T(t1);
        mul_reduce(&prodN1, &prodD1, num_table[t1], den_table[t1]);

        /* paired divisor m1/d if distinct and not m1 */
        idx_t d2 = m1 / d;
        if (d2 != d && d2 != m1) {
            idx_t t2 = d;               /* because m1 / d2 == d */
            compute_T(t2);
            mul_reduce(&prodN1, &prodD1, num_table[t2], den_table[t2]);
        }
    }

    /* Divisors of m2 = n-1 (only if n > 1) */
    seq_t prodN2 = 1, prodD2 = 1;
    if (n > 1) {
        idx_t m2 = n - 1;
        for (idx_t d = 2; d * d <= m2; ++d) {
            if (m2 % d != 0) continue;
            idx_t t1 = m2 / d;
            compute_T(t1);
            mul_reduce(&prodN2, &prodD2, num_table[t1], den_table[t1]);

            idx_t d2 = m2 / d;
            if (d2 != d && d2 != m2) {
                idx_t t2 = d;
                compute_T(t2);
                mul_reduce(&prodN2, &prodD2, num_table[t2], den_table[t2]);
            }
        }
        /* divisor equal to m2 itself gives t=1, which contributes T(1,1)=1 (no effect) */
    }

    /* T(n,1) = n * prodD1 * prodD2 / (prodN1 * prodN2) */
    seq_t numerator   = (seq_t)n * prodD1 * prodD2;
    seq_t denominator = prodN1 * prodN2;

    seq_t g = gcd(numerator, denominator);
    num_table[n] = numerator / g;
    den_table[n] = denominator / g;
}

seq_t a(idx_t n)
{
    compute_T(n);
    return den_table[n];
}
