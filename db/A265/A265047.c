/*
 * A265047  Poincaré series for hyperbolic reflection group with Coxeter
 *          diagram o-(4)-o---o-(5)-o.
 * https://oeis.org/A265047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence derived from the rational
 *            generating function N(x)/D(x) with N(x) and D(x) given in the
 *            OEIS entry. Recurrence (order 15, with dcoeff[0]=1): a(n) =
 *            bcoeff[n] - Σ_{k=1}^{15} dcoeff[k]*a(n-k), where bcoeff[n] =
 *            coefficient of degree n in N(x) (0 if n>15). Initia...
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 63, u64 n <= 135, u128 n <= 279,
 *            big any n
 * Helpers:   alloc_seq_array, poly_mul_ll
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/poly_mul_ll.h"

static long long dcoeff[16];
static long long bcoeff[16];
static int coeffs_initialized = 0;

static void init_coeffs(void)
{
    if (coeffs_initialized) return;

    /* Denominator D(x) = (1 - x)*(1 + x^3 + x^6)*(1 - x + x^2 - 2x^3 + x^4 - 2x^5 + x^6 - x^7 + x^8) */
    const long long p1[] = {1, -1};
    const long long p2[] = {1, 0, 0, 1, 0, 0, 1};
    const long long p3[] = {1, -1, 1, -2, 1, -2, 1, -1, 1};

    long long tmp[8] = {0};
    long long tmp2[16] = {0};
    poly_mul_ll(p1, 2, p2, 7, tmp);          /* (1-x)*(1+x^3+x^6) */
    poly_mul_ll(tmp, 8, p3, 9, tmp2);        /* times P(x) */

    for (size_t i = 0; i < 16; ++i) dcoeff[i] = tmp2[i];

    /* Numerator N(x) = (1+x)^3*(1 -x +x^2)*(1 +x^2)*(1 -x +x^2 -x^3 +x^4)*(1 +x +x^2 +x^3 +x^4) */
    const long long q1[] = {1, 3, 3, 1};
    const long long q2[] = {1, -1, 1};
    const long long q3[] = {1, 0, 1};
    const long long q4[] = {1, -1, 1, -1, 1};
    const long long q5[] = {1, 1, 1, 1, 1};

    long long r0[7] = {0};
    long long r1[9] = {0};
    long long r2[14] = {0};
    long long r3[19] = {0};

    poly_mul_ll(q1, 4, q2, 3, r0);          /* degree 5 */
    poly_mul_ll(r0, 7, q3, 3, r1);          /* degree 7 */
    poly_mul_ll(r1, 9, q4, 5, r2);          /* degree 11 */
    poly_mul_ll(r2, 14, q5, 5, r3);         /* degree 15 */

    for (size_t i = 0; i < 16; ++i) bcoeff[i] = r3[i];

    coeffs_initialized = 1;
}

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    init_coeffs();

    if (idx >= cache_cap) {
        /* grow cache to at least idx+1 */
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap <= idx) new_cap <<= 1;

        seq_t *new_cache = alloc_seq_array(new_cap);

        /* copy old values */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];

        /* compute new entries */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            seq_t sum = (i < 16) ? (seq_t)bcoeff[i] : (seq_t)0;

            for (size_t k = 1; k <= 15; ++k) {
                if (i < k) continue;
                if (dcoeff[k] > 0) {
                    sum -= (seq_t)dcoeff[k] * new_cache[i - k];
                } else if (dcoeff[k] < 0) {
                    sum += (seq_t)(-dcoeff[k]) * new_cache[i - k];
                }
            }

            new_cache[i] = sum;
        }

        /* replace old cache */
        free(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    return cache[idx];
}
