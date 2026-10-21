/*
 * A294819  Number of permutations of [n] avoiding {1324, 2431, 3142}.
 * https://oeis.org/A294819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A294819: Number of permutations of [n] avoiding {1324, 2431, 3142}.
 *
 * The sequence satisfies the linear recurrence with polynomial coefficients:
 *   2*(n+1)*a(n)+(-19*n+3)*a(n-1)+2*(31*n-39)*a(n-2)+2*(-40*n+93)*a(n-3)
 *   +(33*n-107)*a(n-4)+2*(-2*n+9)*a(n-5)=0     for n ≥ 5
 *
 * Initial terms:  a(0)=1, a(1)=1, a(2)=2, a(3)=6, a(4)=21.
 * The recurrence can be rewritten as
 *   a(n) = [ (19*n-3)*a(n-1)+ (62*n-78)*a(n-2)+ (80*n-186)*a(n-3)
 *           + (33*n-107)*a(n-4)+ (4*n-18)*a(n-5) ] / [ 2*(n+1) ]
 *   (valid for n ≥ 5).
 *
 * The implementation uses memoisation to compute values in O(n) time per
 * new maximum n, and works for any idx_t/seq_t types supported by the
 * build system.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation state */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t next_calc = 0;   /* index of first value not yet computed */

    /* Ensure at least capacity for the requested index */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        /* ensure we have space for the five initial values */
        if (new_cap < 5)
            new_cap = 5;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first five values on first entry */
    if (next_calc == 0) {
        cache[0] = (seq_t)1;
        if (cache_cap > 1) cache[1] = (seq_t)1;
        if (cache_cap > 2) cache[2] = (seq_t)2;
        if (cache_cap > 3) cache[3] = (seq_t)6;
        if (cache_cap > 4) cache[4] = (seq_t)21;
        next_calc = 5;   /* values up to 4 are now ready */
    }

    /* Compute missing terms up to n */
    if (next_calc <= n) {
        for (idx_t i = next_calc; i <= n; ++i) {
            /* a(i) depends on the five previous terms */
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];

            /* Compute the polynomial coefficients as seq_t */
            seq_t coeff1 = ((seq_t)19 * i) - (seq_t)3;   /* 19*i-3   */
            seq_t coeff2 = ((seq_t)62 * i) - (seq_t)78;  /* 62*i-78  */
            seq_t coeff3 = ((seq_t)80 * i) - (seq_t)186; /* 80*i-186 */
            seq_t coeff4 = ((seq_t)33 * i) - (seq_t)107; /* 33*i-107 */
            seq_t coeff5 = ((seq_t)4  * i) - (seq_t)18;  /* 4*i-18   */

            /* Positive contributions */
            seq_t term1 = coeff1 * a1;
            seq_t term3 = coeff3 * a3;
            seq_t term5 = coeff5 * a5;

            /* Negative contributions (subtracted) */
            seq_t term2 = coeff2 * a2;
            seq_t term4 = coeff4 * a4;

            /* Numerator of the recurrence */
            seq_t val = term1 + term3 + term5;
            val -= term2;
            val -= term4;

            /* Denominator: 2*(i+1) */
            seq_t denom = (seq_t)2 * (i + (idx_t)1);

            cache[i] = val / denom;   /* Integer division yields a(n) */
        }
        next_calc = n + 1;
    }

    return cache[n];
}
