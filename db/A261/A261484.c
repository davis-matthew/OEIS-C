/*
 * A261484  Number of set partitions of [n] into exactly eight parts such that
 *          no part contains two elements with a circular distance less than
 *          three.
 * https://oeis.org/A261484
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 12, u32 n <= 18, u64 n <= 30, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A261484
   Number of set partitions of [n] into exactly eight parts such that
   no part contains two elements with a circular distance less than three.
   The generating function G(x) is rational:
        G(x) = ((720*x^11 + ... + 4*x + 1) * x^8) /
               ((x-1)(6x-1)(5x-1)(3x-1)(2x-1)(4x-1)(x+1)(x^2+x+1)
                (5x^2+x+1)(3x^2+x+1)(4x^2+x+1)(2x^2+x+1)(6x^2+x+1))
   Since the denominator has constant term 1, the coefficients
   a(n) satisfy the linear recurrence
        a[n] = p[n] - Σ_{k=1}^d q[k] * a[n-k]   (n ≥ 0),
   where p[n] are the coefficients of the numerator polynomial
   (shifted by x^8) and q[k] are the coefficients of the denominator.
   All values with n < 8 are zero. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#define NUM_SIZE 20          /* degree of numerator (x^8 * (deg 11 polynomial)) */
#define DEN_D 19             /* degree of the denominator */

/* Numerator coefficients (P(x) = (720x^11 + … + 4x + 1) * x^8). */
static const seq_t NUMERATOR[NUM_SIZE] = {
    0, 0, 0, 0, 0, 0, 0, 0,   /* x^0 … x^7  */
    1,    /* x^8  */
    4,    /* x^9  */
    28,   /* x^10 */
    69,   /* x^11 */
    262,  /* x^12 */
    392,  /* x^13 */
    1105, /* x^14 */
    755,  /* x^15 */
    2204, /* x^16 */
    580,  /* x^17 */
    720,  /* x^18 */
    720   /* x^19 */
};

/* Denominator coefficients q[0..DEN_D] (q[0] = 1). */
static signed long long DEN_COEF[DEN_D + 1] = {0};
static bool DEN_INI = false;

/* Cache array for a[n] */
static seq_t *CACHE = NULL;
static size_t CACHE_CAP = 0;
static idx_t CACHE_MAX = 0;   /* largest n already computed */

/* Multiply two polynomials stored in low‑to‑high coefficient arrays.
   Result is stored in res (must be large enough to hold len1+len2-1 terms). */
static void poly_mul(const signed long long *p1, size_t len1,
                     const signed long long *p2, size_t len2,
                     signed long long *res)
{
    for (size_t i = 0; i < len1 + len2 - 1; ++i)
        res[i] = 0;
    for (size_t i = 0; i < len1; ++i)
        for (size_t j = 0; j < len2; ++j)
            res[i + j] += p1[i] * p2[j];
}

/* Construct the denominator polynomial. */
static void build_denominator(void)
{
    signed long long temp[20] = {0};
    signed long long tmp[20] = {0};

    /* Start from the constant polynomial 1. */
    temp[0] = 1;
    size_t deg = 0;

    struct factor { size_t len; signed long long coeff[3]; } f[] = {
        {2, {-1,  1}},   /* (x - 1)          */
        {2, {-1,  6}},   /* (6x - 1)         */
        {2, {-1,  5}},   /* (5x - 1)         */
        {2, {-1,  3}},   /* (3x - 1)         */
        {2, {-1,  2}},   /* (2x - 1)         */
        {2, {-1,  4}},   /* (4x - 1)         */
        {2, { 1,  1}},   /* (x + 1)          */
        {3, { 1,  1,  1}}, /* (x^2 + x + 1)    */
        {3, { 1,  1,  5}}, /* (5x^2 + x + 1)   */
        {3, { 1,  1,  3}}, /* (3x^2 + x + 1)   */
        {3, { 1,  1,  4}}, /* (4x^2 + x + 1)   */
        {3, { 1,  1,  2}}, /* (2x^2 + x + 1)   */
        {3, { 1,  1,  6}}  /* (6x^2 + x + 1)   */
    };
    const size_t NF = sizeof(f) / sizeof(f[0]);

    for (size_t i = 0; i < NF; ++i) {
        poly_mul(temp, deg + 1, f[i].coeff, f[i].len, tmp);
        deg += f[i].len - 1;
        for (size_t j = 0; j <= deg; ++j)
            temp[j] = tmp[j];
    }

    for (size_t i = 0; i <= DEN_D; ++i)
        DEN_COEF[i] = temp[i];
    DEN_INI = true;
}

/* Ensure CACHE has capacity at least new_cap. */
static void ensure_cache_capacity(size_t new_cap)
{
    if (new_cap <= CACHE_CAP)
        return;
    size_t new_size = CACHE_CAP ? CACHE_CAP : 64;
    while (new_size < new_cap)
        new_size <<= 1;
    CACHE = (seq_t *)realloc(CACHE, new_size * sizeof(seq_t));
    CACHE_CAP = new_size;
}

/* Main function: compute a(n). */
seq_t a(idx_t n)
{
    if (n < 8)          /* values below offset are not defined */
        return (seq_t)0;

    if (!DEN_INI) {
        build_denominator();
        ensure_cache_capacity(64);
        for (size_t i = 0; i < 8; ++i)
            CACHE[i] = (seq_t)0;
        CACHE_MAX = 7;
    }

    if ((idx_t)CACHE_MAX < n) {
        ensure_cache_capacity((size_t)n + 1);
        for (idx_t m = CACHE_MAX + 1; m <= n; ++m) {
            seq_t sum = (m < NUM_SIZE) ? NUMERATOR[(size_t)m] : (seq_t)0;

            /* First add contributions from negative coefficients. */
            for (int k = 1; k <= DEN_D && k <= m; ++k) {
                if (DEN_COEF[k] < 0) {
                    unsigned long long abscoef = (unsigned long long)(-DEN_COEF[k]);
                    seq_t prod = CACHE[m - k] * (seq_t)abscoef;
                    sum += prod;   /* guaranteed not to underflow */
                }
            }
            /* Then subtract contributions from positive coefficients. */
            for (int k = 1; k <= DEN_D && k <= m; ++k) {
                if (DEN_COEF[k] > 0) {
                    unsigned long long abscoef = (unsigned long long)DEN_COEF[k];
                    seq_t prod = CACHE[m - k] * (seq_t)abscoef;
                    sum -= prod;   /* safe because sum already large enough */
                }
            }
            CACHE[m] = sum;
        }
        CACHE_MAX = n;
    }
    return CACHE[n];
}
