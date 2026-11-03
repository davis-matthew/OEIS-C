/*
 * A328725  Constant term in the expansion of (1 + x + y + z + 1/x + 1/y + 1/z
 *          + x*y + y*z + z*x + 1/(x*y) + 1/(y*z) + 1/(z*x) + x*y*z +
 *          1/(x*y*z))^n.
 * https://oeis.org/A328725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n} (-1)^(n-i)*binomial(n,i)*Sum_{j=0..i}
 *            binomial(i,j)^4 * Recurrence: * n^3*a(n) = (2*n - 1)^3*a(n-1) *
 *            + (n-1)*(94*n^2 - 188*n + 93)*a(n-2) * + 80*(n-2)*(n-1)*(2*n -
 *            3)*a(n-3) * + 75*(n-3)*(n-2)*(n-1)*a(n-4) * Initial values: *
 *            a(0)=1, a(1)=1, a(2)=15, a(3)=115, a(4)=1255
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 801 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Big‑integer branch – uses seq_t for all operations */
static seq_t compute_term_big(idx_t i, seq_t *array)
{
    seq_t n = (seq_t)i;

    seq_t two_n_minus1 = (seq_t)2 * n - 1;
    seq_t coeff1 = two_n_minus1 * two_n_minus1 * two_n_minus1;

    seq_t coeff2 = (n - 1) *
        (((seq_t)94 * n * n) - ((seq_t)188 * n) + (seq_t)93);

    seq_t coeff3 = (seq_t)80 * (n - 2) * (n - 1) *
        ((seq_t)2 * n - 3);

    seq_t coeff4 = (seq_t)75 * (n - 3) * (n - 2) * (n - 1);

    seq_t denom = n * n * n;

    seq_t rhs =
        coeff1 * array[i - 1] +
        coeff2 * array[i - 2] +
        coeff3 * array[i - 3] +
        coeff4 * array[i - 4];

    return rhs / denom;
}

/* Small‑integer branch – uses 128‑bit intermediate */
static seq_t compute_term_small(idx_t i, seq_t *array)
{
    unsigned long long n = (unsigned long long)i;

    /* coefficients (64‑bit) */
    unsigned long long t = 2 * n - 1;
    __uint128_t coeff1 = (__uint128_t)t * t * t;

    unsigned long long coeff2_val = (n - 1) *
        ((94 * n * n) - (188 * n) + 93);
    __uint128_t coeff2 = coeff2_val;

    unsigned long long coeff3_val = 80 * (n - 2) * (n - 1) * (2 * n - 3);
    __uint128_t coeff3 = coeff3_val;

    unsigned long long coeff4_val = 75 * (n - 3) * (n - 2) * (n - 1);
    __uint128_t coeff4 = coeff4_val;

    /* intermediate products (128‑bit) */
    __uint128_t term1 = coeff1 * (unsigned long long)array[i - 1];
    __uint128_t term2 = coeff2 * (unsigned long long)array[i - 2];
    __uint128_t term3 = coeff3 * (unsigned long long)array[i - 3];
    __uint128_t term4 = coeff4 * (unsigned long long)array[i - 4];

    __uint128_t rhs = term1 + term2 + term3 + term4;
    __uint128_t denom = (__uint128_t)n * n * n;

    return (seq_t)(rhs / denom);
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap = 0;   /* number of entries already stored */

    if (n >= cap) {
        idx_t newcap = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, newcap);
        if (!tmp) abort();   /* allocation failed */
        for (idx_t i = cap; i <= n; ++i) {
            seq_t val;
            if (i <= 4) {
                switch ((int)i) {
                    case 0: case 1: val = (seq_t)1; break;
                    case 2: val = (seq_t)15; break;
                    case 3: val = (seq_t)115; break;
                    case 4: val = (seq_t)1255; break;
                    default: val = (seq_t)0; /* unreachable */
                }
            } else {
                if (sizeof(seq_t) <= sizeof(unsigned long long))
                    val = compute_term_small(i, tmp);
                else
                    val = compute_term_big(i, tmp);
            }
            tmp[i] = val;
        }
        cache = tmp;
        cap   = newcap;
    }
    return cache[n];
}
