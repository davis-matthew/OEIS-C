/*
 * A259888  Number of (n+2) X (2+2) 0..1 arrays with each 3 X 3 subblock
 *          having clockwise perimeter pattern 00000000 00000001 or 00000011.
 * https://oeis.org/A259888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 18, u64 n <= 39, u128 n <= 81,
 *            big any n
 * Helpers:   mat_mult, mat_pow, mat_vec_mul, mul11, mul8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A259888: Number of (n+2)×(2+2) 0..1 arrays with each 3×3 subblock having clockwise perimeter pattern 00000000 00000001 or 00000011.
   Recurrence: a(n) = 3*a(n-1) - 3*a(n-2) + 11*a(n-3) - 8*a(n-4) - 5*a(n-5) - 5*a(n-6) - 2*a(n-7) + 3*a(n-8)
   Initial terms (n=1..8): 67, 174, 653, 1853, 4662, 13704, 40955, 114098
   This implementation caches terms up to index 10000 using the recurrence,
   and for larger indices applies a fast 8‑term linear‑recurrence
   exponentiation with the companion matrix.  The function works for
   any seq_t that supports the usual arithmetic operators.  */

#include <stddef.h>
#include <stdint.h>
#include "../utils/mat_mult.h"
#include "../utils/mat_pow.h"
#include "../utils/mat_vec_mul.h"
#include "../utils/mul11.h"
#include "../utils/mul8.h"

static seq_t memo[10001];          /* cache for indices 1..10000 */
static int  max_computed = 0;      /* highest index computed in the cache */
static int  cache_init   = 0;      /* flag if initial values have been cached */

static inline seq_t mul3(const seq_t *x) { return (*x) * (seq_t)3; }
static inline seq_t mul5(const seq_t *x) { return (*x) * (seq_t)5; }

static inline seq_t mul2(const seq_t *x) { return (*x) * (seq_t)2; }

/* Compute next term from the recurrence; all intermediate subtractions
   are safe because the recurrence always yields a non‑negative value. */
static seq_t next_term(idx_t i)
{
    seq_t r = 3 * memo[i-1];
    r -= 3 * memo[i-2];
    r += 11 * memo[i-3];
    r -= 8 * memo[i-4];
    r -= 5 * memo[i-5];
    r -= 5 * memo[i-6];
    r -= 2 * memo[i-7];
    r += 3 * memo[i-8];
    return r;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;   /* undefined for n=0 */

    /* Initialize cache with first 8 terms if not done yet */
    if (!cache_init) {
        static const seq_t init[8] = {
            (seq_t)67,  (seq_t)174, (seq_t)653,  (seq_t)1853,
            (seq_t)4662,(seq_t)13704,(seq_t)40955,(seq_t)114098
        };
        for (idx_t i = 1; i <= 8; ++i)
            memo[i] = init[i-1];
        max_computed = 8;
        cache_init = 1;
    }

    /* If n is within the cached range */
    if (n <= (idx_t)10000) {
        if ((idx_t)max_computed < n) {
            for (idx_t i = (idx_t)(max_computed + 1); i <= n; ++i)
                memo[i] = next_term(i);
            max_computed = (int)n;
        }
        return memo[n];
    }

    /* For n > 10000 use fast exponentiation of the companion matrix */
    if (n <= (idx_t)8)
        return memo[n];

    /* Build companion matrix */
    seq_t M[8][8] = {0};
    /* First row: coefficients of recurrence */
    M[0][0] = (seq_t)3;  /* +3*a(n-1) */
    M[0][1] = (seq_t)(-3); /* -3*a(n-2) */
    M[0][2] = (seq_t)11; /* +11*a(n-3) */
    M[0][3] = (seq_t)(-8); /* -8*a(n-4) */
    M[0][4] = (seq_t)(-5); /* -5*a(n-5) */
    M[0][5] = (seq_t)(-5); /* -5*a(n-6) */
    M[0][6] = (seq_t)(-2); /* -2*a(n-7) */
    M[0][7] = (seq_t)3;  /* +3*a(n-8) */

    /* Sub‑diagonal ones */
    for (size_t k = 1; k < 8; ++k)
        M[k][k-1] = (seq_t)1;

    /* Exponentiate matrix to the (n-8)th power */
    mat_pow(M, n - (idx_t)8);

    /* Vector of the last 8 terms: [a8, a7, a6, a5, a4, a3, a2, a1] */
    seq_t v[8];
    for (idx_t i = 0; i < 8; ++i)
        v[i] = memo[8 - i];

    /* Compute a(n) = first row of M * v */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < 8; ++i)
        result += M[0][i] * v[i];

    return result;
}
