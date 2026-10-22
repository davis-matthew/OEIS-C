/*
 * A309543  Number of odd parts in the partitions of n into 5 parts.
 * https://oeis.org/A309543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   int_mul_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A309543: number of odd parts in the partitions of n into 5 parts
 *
 * The generating function is a rational function
 *   -x^5*(2*x^11-x^10+2*x^8+4*x^6-4*x^5+5*x^4-2*x^3+5*x^2-6*x+5)
 *   / ((x^2+1) *(x^2+x+1) *(x^2-x+1) *(x^4+x^3+x^2+x+1) *(x^4-x^3+x^2-x+1)
 *      *(x^4+1) *(x+1)^3 *(x-1)^5).
 *
 * Expanding the denominator gives a linear recurrence of order 26:
 *   a(n) = c1*a(n-1) + c2*a(n-2) + ... + c26*a(n-26)   for n>=26.
 *
 * The initial values a(0)..a(25) are obtained by brute‑force evaluation of the
 * quadruple sum in the definition.  Once the recurrence is known the
 * sequence can be generated in O(n) time.  All arithmetic is performed in
 * signed 64‑bit to avoid overflow during coefficient application; the
 * result is stored in seq_t, possibly an unsigned 8‑, 16‑, 32‑, 64‑,
 * 128‑bit integer or arbitrary‑precision type.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/int_mul_floor.h"

/* Helper to multiply two polynomials a[0..degA] and b[0..degB], result
 * stored in out[0..degA+degB] (out must have that many elements).  All
 * arguments are signed 64‑bit. */
static void poly_mul(long long *out, const long long *a, size_t degA,
                     const long long *b, size_t degB)
{
    size_t degOut = degA + degB;
    for (size_t i = 0; i <= degOut; ++i) out[i] = 0;
    for (size_t i = 0; i <= degA; ++i)
        if (a[i])
            for (size_t j = 0; j <= degB; ++j)
                if (b[j])
                    out[i + j] += a[i] * b[j];
}

/* Compute the sequence value a(n).  The implementation follows the
 * recurrence derived from the generating function and precomputes
 * the initial values (for indices < 26).  It keeps a static cache of all
 * computed values so repeated calls are O(1). */
seq_t a(idx_t n)
{
    /* --- Recurrence order and static arrays -------------------------------- */
    enum { R = 26 };                          /* order of recurrence */
    static seq_t init[R];                     /* initial values 0..R-1 */
    static long long coeff[R + 1];             /* denominator coefficients, coeff[0] = -1 */

    /* --- Cached sequence and its current length -------------------------- */
    static idx_t cache_len = 0;
    static seq_t *cache = NULL;
    static int initialized = 0;

    /* ----- One‑time initialization ---------------------------------------- */
    if (!initialized) {
        /* Build the denominator polynomial as the product of its factors. */
        long long poly[27] = {0};             /* enough for degree 26 */
        size_t deg = 0;
        poly[0] = 1;                          /* start with 1 */

        /* Factor polynomials.  Each is given in coefficient order. */
        const long long f1[] = {1,0,1};                              /* x^2 + 1     */
        const long long f2[] = {1,1,1};                              /* x^2 + x + 1 */
        const long long f3[] = {1,-1,1};                             /* x^2 - x + 1 */
        const long long f4[] = {1,1,1,1,1};                          /* x^4 + x^3 + x^2 + x + 1 */
        const long long f5[] = {1,-1,1,-1,1};                        /* x^4 - x^3 + x^2 - x + 1 */
        const long long f6[] = {1,0,0,0,1};                          /* x^4 + 1     */
        const long long f7[] = {1,3,3,1};                            /* (x+1)^3     */
        const long long f8[] = {-1,5,-10,10,-5,1};                   /* (x-1)^5     */

        /* Macro to multiply current polynomial by a new factor.  */
        /* This keeps the code readable and avoids manual allocation. */
#define MUL_FACTOR(f, d) \
        do { \
            long long tmp[27] = {0}; \
            poly_mul(tmp, poly, deg, f, d); \
            for (size_t i = 0; i <= deg + d; ++i) poly[i] = tmp[i]; \
            deg += d; \
        } while (0)

        MUL_FACTOR(f1, 2);
        MUL_FACTOR(f2, 2);
        MUL_FACTOR(f3, 2);
        MUL_FACTOR(f4, 4);
        MUL_FACTOR(f5, 4);
        MUL_FACTOR(f6, 4);
        MUL_FACTOR(f7, 3);
        MUL_FACTOR(f8, 5);

        /* Store coefficients in coeff[0..R].  coeff[0] should be -1. */
        for (int i = 0; i <= (int)deg; ++i) coeff[i] = poly[i];
        /* Ensure correct order. */
        /* coeff[0] = -1 (constant term) by construction. */

        /* ---------------------------------- Evaluate initial values.  */
        for (idx_t val = 0; val < (idx_t)R; ++val) {
            seq_t sum = 0;
            if (val >= 5) {
                for (idx_t l = 1; l <= val / 5; ++l) {
                    idx_t maxK = (val - 1) / 4;
                    for (idx_t k = l; k <= maxK; ++k) {
                        idx_t rem1 = val - k - l;
                        if (rem1 < 0) continue;
                        idx_t maxJ = rem1 / 3;
                        for (idx_t j = k; j <= maxJ; ++j) {
                            idx_t rem2 = rem1 - j;
                            if (rem2 < 0) continue;
                            idx_t maxI = rem2 / 2;
                            for (idx_t i = j; i <= maxI; ++i) {
                                idx_t rem3 = rem2 - i;
                                seq_t odd =
                                    (i & 1) + (j & 1) + (k & 1) + (l & 1) + (rem3 & 1);
                                sum += odd;
                            }
                        }
                    }
                }
            }
            init[val] = sum;
        }

        /* Allocate cache for the initial values. */
        cache_len = R;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i) cache[i] = init[i];

        initialized = 1;
    }

    /* --- Extend the cache if necessary ----------------------------------- */
    if (n >= cache_len) {
        idx_t new_len = cache_len ? cache_len * 2 : 32;
        while (new_len <= n) new_len *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = tmp;
        /* Compute new values using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i) {
            if (i < (idx_t)R) {
                /* Should already be in cache, but keep for safety. */
                cache[i] = init[i];
            } else {
                int64_t s = 0;
                for (int k = 1; k <= R; ++k)
                    s += coeff[k] * (int64_t)cache[i - k];
                cache[i] = (seq_t)s;        /* truncates/overflows naturally */
            }
        }
        cache_len = n + 1;
    }

    return cache[n];
}
