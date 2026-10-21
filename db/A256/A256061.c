/*
 * A256061  Number T(n,k) of 2n-length strings of balanced parentheses of
 *          exactly k different types; triangle T(n,k), n>=0, 0<=k<=n, read by
 *          rows.
 * https://oeis.org/A256061
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, k) where (row, k) are the coordinates of the
 *            flattened triangle of T(n,k) = Σ_{i=0..k} (-1)^i * C(k,i) *
 *            (k-i)^n * Catalan(n). The triangle is traversed row‑major with
 *            rows (n,k) for 0 ≤ k ≤ n. For n=0 the triangle consists of the
 *            single entry T(0,0)=1
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 50, u64 n <= 126,
 *            u128 n <= 315, big any n
 * Helpers:   big_pow, binomial, index_to_row_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include helper functions that provide arbitrary‑precision arithmetic */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "../utils/binomial.h"   /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/big_pow.h"    /* seq_t big_pow(seq_t base, seq_t exp); */
#include "../utils/index_to_row_k.h"

/* Compute Catalan(n) = C(2n,n)/(n+1) using the provided binomial. */
static seq_t catalan(idx_t n)
{
    seq_t num = binomial(2 * n, n);      /* C(2n,n) */
    seq_t den = (seq_t)(n + 1);          /* n+1 */
    return num / den;                    /* exact division */
}

/* Compute T(n,k) via inclusion–exclusion. */
static seq_t triangle_T(idx_t n, idx_t k)
{
    if (k > n) return 0;                 /* safety guard */
    seq_t sum = 0;
    seq_t catalan_n = catalan(n);

    /* Compute binomial coefficients C(k,i) iteratively. */
    seq_t C = 1;                           /* C(k,0) */
    for (idx_t i = 0; i <= k; ++i) {
        /* (k-i)^n */
        seq_t base_pow;
        if (n == 0) {
            base_pow = 1;                 /* convention 0^0 = 1 */
        } else if ((k == i) && n > 0) {
            base_pow = 0;                 /* base 0, exponent >0 */
        } else {
            base_pow = big_pow((seq_t)(k - i), (seq_t)n);
        }

        /* term = C(k,i) * base_pow * Catalan(n) */
        seq_t term = C * base_pow;
        term = term * catalan_n;

        /* Add or subtract depending on parity of i. */
        if (i & 1) {
            sum -= term;
        } else {
            sum += term;
        }

        /* Update C to C(k,i+1) for next iteration. */
        if (i < k) {
            C = C * ((seq_t)(k - i));
            C = C / ((seq_t)(i + 1));
        }
    }
    return sum;
}

/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    idx_t row, k;
    index_to_row_k(n, &row, &k);
    return triangle_T(row, k);
}
