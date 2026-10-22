/*
 * A318649  Numerators of the sequence whose Dirichlet convolution with itself
 *          yields squares, A000290.
 * https://oeis.org/A318649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 80, u32 n <= 5102, u64 n <= 9185,
 *            u128 n <= 9185, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A318649
 *  Formula: a(n) = numerator of f(n)
 *  where f(1) = 1, and for n > 1
 *  f(n) = (1/2) * ( n^2 - Sum_{d|n, 1<d<n} f(d) * f(n/d) )
 *  The values are stored reduced so that the denominator is a power of 2.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* static storage for the computed values */
static seq_t *f_num   = NULL;   /* numerator of f(n) */
static int   *f_exp   = NULL;   /* exponent of 2 in the denominator of f(n) */
static unsigned char *f_seen = NULL;   /* 0 = not computed, 1 = computed */
static idx_t   f_cap = 0;                /* allocated size */

/* enlarge storage to at least n+1 elements */
static void ensure_cache(idx_t n)
{
    if (n < f_cap) return;
    size_t new_cap = f_cap ? f_cap << 1 : 1;
    while (new_cap <= n) new_cap <<= 1;
    f_num   = (seq_t *)   realloc(f_num,   new_cap * sizeof(seq_t));
    f_exp   = (int  *)    realloc(f_exp,   new_cap * sizeof(int));
    f_seen  = (unsigned char *)realloc(f_seen, new_cap * sizeof(unsigned char));
    for (size_t i = f_cap; i < new_cap; ++i) f_seen[i] = 0;
    f_cap = new_cap;
}

/* forward declaration */
static void compute_f(idx_t n);

/* main function requested by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for out-of‑range */
    ensure_cache(n);
    if (!f_seen[n]) compute_f(n);
    return f_num[n];
}

/* recursively compute f(n) */
static void compute_f(idx_t n)
{
    if (f_seen[n]) return;             /* already computed */
    if (n == 1) {                      /* base case */
        f_num[1] = (seq_t)1;
        f_exp[1] = 0;
        f_seen[1] = 1;
        return;
    }

    /* sum over proper divisors d:   1 < d < n  */
    seq_t sum_num = 0;                 /* numerator of the sum   */
    int   sum_exp = 0;                 /* exponent of 2 in sum   */

    for (idx_t i = 2; i * i <= n; ++i) if (n % i == 0) {
        /* first divisor in the pair */
        idx_t d1 = i;
        if (d1 > 1 && d1 < n) {
            compute_f(d1);
            idx_t d2 = n / d1;                 /* second divisor in the pair */
            compute_f(d2);

            /* product f(d1)*f(d2) */
            seq_t prod_num = f_num[d1] * f_num[d2];
            int   prod_exp = f_exp[d1] + f_exp[d2];

            /* add to the running sum */
            if (sum_num == 0) {
                sum_num = prod_num;
                sum_exp = prod_exp;
            } else {
                if (prod_exp > sum_exp) {
                    sum_num <<= (prod_exp - sum_exp);
                    sum_exp = prod_exp;
                } else if (prod_exp < sum_exp) {
                    prod_num <<= (sum_exp - prod_exp);
                }
                sum_num += prod_num;
            }
        }

        /* second divisor in the pair, unless it equals d1 */
        idx_t d2 = n / i;
        if (d2 != i && d2 > 1 && d2 < n) {
            compute_f(d2);
            idx_t d1 = i;                     /* d1 again for clarity */
            compute_f(d1);

            seq_t prod_num = f_num[d1] * f_num[d2];
            int   prod_exp = f_exp[d1] + f_exp[d2];

            if (sum_num == 0) {
                sum_num = prod_num;
                sum_exp = prod_exp;
            } else {
                if (prod_exp > sum_exp) {
                    sum_num <<= (prod_exp - sum_exp);
                    sum_exp = prod_exp;
                } else if (prod_exp < sum_exp) {
                    prod_num <<= (sum_exp - prod_exp);
                }
                sum_num += prod_num;
            }
        }
    }

    /* n^2 as an integer (denominator 1) */
    seq_t n2_int = (seq_t)n * (seq_t)n;

    /* bring n^2 to the denominator 2^sum_exp */
    seq_t n2_shifted = n2_int << sum_exp;

    /* compute (n^2 - sum) */
    seq_t diff_num = n2_shifted - sum_num;
    int   diff_exp = sum_exp + 1;         /* extra division by 2 */

    /* cancel common powers of 2 */
    while ((diff_num & (seq_t)1) == 0 && diff_exp > 0) {
        diff_num >>= 1;
        diff_exp--;
    }

    /* store result */
    f_num[n] = diff_num;
    f_exp[n] = diff_exp;
    f_seen[n] = 1;
}
