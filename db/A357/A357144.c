/*
 * A357144  Square array, A(n, k), n, k >= 0, read by antidiagonals; A(n, k) =
 *          g(f(n) * f(k)) where f(m) = A002487(m)/A002487(m+1) and g is the
 *          inverse of f.
 * https://oeis.org/A357144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A(⌊k⌋, ⌊l⌋) where A(i,j) = g(f(i) * f(j)), f(m) =
 *            Stern(m)/Stern(m+1), g is the inverse of f. A002487(m) is
 *            Stern's diatomic sequence: a(0)=0, a(1)=1, a(2n)=a(n),
 *            a(2n+1)=a(n)+a(n+1). This implementation builds Stern up to the
 *            required indices, multiplies the corresponding fractions,
 *            reduc...
 * Range:     u8 n <= 39, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   antidiagonal_coords, cf_inverse, seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/antidiagonal_coords.h"
#include "../utils/seq_gcd.h"
#include "../utils/cf_inverse.h"

/* Cache for Stern's diatomic sequence, 0‑based. */
static seq_t *stern_cache = NULL;  /* array of Stern values */
static idx_t   stern_size  = 0;     /* number of elements cached (exclusive) */
static idx_t   stern_cap   = 0;     /* allocated capacity */

/* Ensure that Stern values up to index m are cached (m inclusive). */
static void ensure_stern(idx_t m)
{
    if (stern_cache == NULL) {
        stern_cap = 16;
        stern_cache = (seq_t *)malloc(stern_cap * sizeof(seq_t));
        stern_cache[0] = (seq_t)0;
        stern_cache[1] = (seq_t)1;
        stern_size   = 2;
    }

    if (m < stern_size) return;

    if ((idx_t)stern_cap < m + 1) {
        stern_cap = ((m + 2) * 2 > (idx_t)UINT32_MAX ? (idx_t)UINT32_MAX : (m + 2) * 2);
        stern_cache = (seq_t *)realloc(stern_cache, stern_cap * sizeof(seq_t));
    }

    for (idx_t i = stern_size; i <= m; ++i) {
        if ((i & 1) == 0)
            stern_cache[i] = stern_cache[i / 2];
        else
            stern_cache[i] = stern_cache[i / 2] + stern_cache[i / 2 + 1];
    }
    stern_size = m + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    /* Rows or columns that contain a zero entry are handled immediately. */
    if (i == 0 || j == 0) return (seq_t)0;

    /* Ensure Stern values are available. */
    ensure_stern(i + 1);
    ensure_stern(j + 1);

    /* Retrieve fractions f(i) = S(i)/S(i+1), f(j) = S(j)/S(j+1). */
    seq_t num1 = stern_cache[i];
    seq_t den1 = stern_cache[i + 1];
    seq_t num2 = stern_cache[j];
    seq_t den2 = stern_cache[j + 1];

    /* Multiply the rationals: num/den = (num1*num2)/(den1*den2). */
    seq_t num = num1 * num2;
    seq_t den = den1 * den2;

    /* Reduce to lowest terms. */
    seq_t g = seq_gcd(num, den);
    num /= g;
    den /= g;

    /* Map the reduced fraction back to the index using the inverse of f. */
    return (seq_t)cf_inverse(num, den);
}
