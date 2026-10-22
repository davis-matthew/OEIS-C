/*
 * A320859  Powers of 2 with initial digit 3.
 * https://oeis.org/A320859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 2^A172404(n) – powers of 2 whose initial digit is 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 15,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/pow2.h"

/* dynamic array of exponents k such that the leading digit of 2^k is 3 */
static idx_t *exponents = NULL;
static size_t exp_size = 0, exp_cap = 0;
static idx_t last_k = 0;
static const double LOG10_2 = 0.30102999566398119521;

/* grow the exponent buffer to hold at least 'need' entries */
static void grow_exp(size_t need)
{
    if (exp_cap >= need)
        return;
    size_t new_cap = exp_cap ? exp_cap * 2 : 256;
    while (new_cap < need)
        new_cap <<= 1;
    exponents = (idx_t *)realloc(exponents, new_cap * sizeof(idx_t));
    exp_cap = new_cap;
}

/* generate exponents until we have at least 'n' of them */
static void generate_until(idx_t n)
{
    while (exp_size < n)
    {
        ++last_k;                       /* next exponent to test */
        double x = last_k * LOG10_2;     /* k * log10(2) */
        double f = x - floor(x);        /* fractional part */
        double p = pow(10.0, f) + 1e-12;/* 10^fractional_part + epsilon */
        int leading = (int)p;           /* first digit (floor) */
        if (leading == 3)
        {
            grow_exp(exp_size + 1);
            exponents[exp_size++] = last_k;
        }
    }
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined for offset < 1 */
    if (exponents == NULL)
    {
        exp_cap = 0;
        exp_size = 0;
        exponents = NULL;
        last_k    = 0;
    }
    generate_until(n);
    idx_t k = exponents[n - 1];
    return pow2(k);
}
