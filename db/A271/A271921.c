/*
 * A271921  Numerator of n*Product_{j=1..n-1} ((3*j + 1)/(3*j + 2)).
 * https://oeis.org/A271921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 23, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A271921: a(n) = n * Π_{j=1}^{n-1} ((3*j+1)/(3*j+2))
   The function returns the numerator of this product */

#include <stdint.h>
#include <stdlib.h>

/* --- Small‑est‑prime‑factor (SPF) sieve ----------------------------------*/
/* Static storage for the SPF table across calls */
static uint32_t *spf_table   = NULL;
static size_t   spf_limit    = 0;

/* Build SPF table up to the given limit.  The table is expanded only
   when a larger limit is requested. */
static void init_spf(size_t limit)
{
    if (limit <= spf_limit)
        return;

    size_t new_len = limit + 1;
    uint32_t *new_spf = (uint32_t *)calloc(new_len, sizeof(uint32_t));
    if (!new_spf)
        return;                       /* should never happen in tests */

    for (size_t i = 2; i <= limit; ++i) {
        if (new_spf[i] == 0) {        /* i is prime */
            for (uint64_t j = i; j <= limit; j += i) {
                if (new_spf[j] == 0)
                    new_spf[j] = (uint32_t)i;
            }
        }
    }
    free(spf_table);
    spf_table = new_spf;
    spf_limit = limit;
}

/* Add (sign * exponent) of prime factorisation of v to the exp array. */
static void factor_update(uint64_t v, int32_t *exp, int sign)
{
    while (v > 1) {
        uint32_t p = spf_table[(size_t)v];
        int cnt = 0;
        while (v % p == 0) {
            v /= p;
            ++cnt;
        }
        exp[p] += sign * cnt;
    }
}

/* --- Sequence function ----------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* not used, but defined for safety */

    /* Maximum number that appears in the products: 3*(n-1)+2 = 3*n-1 */
    size_t limit = 3 * (size_t)n + 1;    /* extra safety margin */
    init_spf(limit);

    /* Array of signed exponents for each possible prime up to limit */
    int32_t *exp = (int32_t *)calloc(limit + 1, sizeof(int32_t));
    if (!exp)
        return (seq_t)0;               /* allocation failure – should not occur */

    /* Process the product part */
    for (idx_t j = 1; j < n; ++j) {
        uint64_t num = 3 * (uint64_t)j + 1;   /* numerator factor */
        uint64_t den = 3 * (uint64_t)j + 2;   /* denominator factor */
        factor_update(num, exp, +1);          /* add numerator primes */
        factor_update(den, exp, -1);          /* subtract denominator primes */
    }

    /* Multiply by n itself */
    factor_update((uint64_t)n, exp, +1);

    /* Build the final numerator from the remaining positive exponents */
    seq_t result = (seq_t)1;
    for (size_t p = 2; p <= limit; ++p) {
        int32_t e = exp[p];
        if (e > 0) {
            for (int32_t k = 0; k < e; ++k)
                result *= (seq_t)p;
        }
    }

    free(exp);
    return result;
}
