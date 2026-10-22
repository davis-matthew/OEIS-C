/*
 * A384282  a(n) is the n-th q-Catalan number for q=n.
 * https://oeis.org/A384282
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = q‑Catalan number with q = n a(n) = [2n n]_n / (1 + n + n²
 *            + … + nⁿ)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/big_pow.h"

/* Compute the n‑th term of OEIS A384282. */
seq_t a(idx_t n)
{
    /* Handle the first two trivial cases */
    if (n <= 1) return 1;

    seq_t q = (seq_t)n;               /* q is the base (q = n)           */
    seq_t result = 1;                 /* running product                */
    seq_t denom_pow = 1;               /* n^i for the denominator part   */
    /* n^(2n) is needed for the first numerator; compute it once */
    seq_t num_pow = big_pow(q, (seq_t)(2 * n));  /* n^(2n)   */

    for (idx_t i = 1; i <= n; ++i) {
        /* Update denominator power: n^i */
        denom_pow *= q;
        seq_t denominator = denom_pow - 1;

        /* Current numerator: n^(2n+1-i) - 1 */
        seq_t numerator = num_pow - 1;

        /* Cancel any common factor between current result and denominator */
        seq_t g = gcd(result, denominator);
        if (g != 1) {
            result /= g;
            denominator /= g;
        }

        /* Cancel any common factor between numerator and denominator */
        g = gcd(numerator, denominator);
        if (g != 1) {
            numerator /= g;
            denominator /= g;
        }

        /* Multiply by the numerator and divide by the remaining denominator */
        result *= numerator;
        result /= denominator;

        /* Prepare num_pow for the next iteration (except after the last one) */
        if (i < n) num_pow /= q;
    }

    /* After the loop num_pow == n^(n+1)   */
    seq_t numer_final = q - 1;      /* (n - 1)                     */
    seq_t denom_final = num_pow - 1;/* n^(n+1) - 1                  */

    /* Final cancellation with the running result */
    seq_t g = gcd(result, denom_final);
    if (g != 1) {
        result /= g;
        denom_final /= g;
    }

    /* Final numerator may share a factor with denom_final */
    g = gcd(numer_final, denom_final);
    if (g != 1) {
        numer_final /= g;
        denom_final /= g;
    }

    result *= numer_final;
    result /= denom_final;

    return result;
}
