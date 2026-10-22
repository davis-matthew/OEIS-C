/*
 * A371772  a(n) = Sum_{k=0..floor(n/3)} binomial(5*n-3*k-1,n-3*k).
 * https://oeis.org/A371772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = Sum_{k=0..floor(n/3)} binomial(5*n-3*k-1, n-3*k)
 * This implementation uses a safe multiplicative binomial routine
 * that avoids overflow by canceling common factors at each step.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Compute binomial(n, k) safely using reductions to avoid overflow.
 * The algorithm uses the standard multiplicative formula while
 * cancelling common factors with the running result and the
 * current denominator. It works for all the integer types used
 * for seq_t (uint8_t … up to arbitrary precision). */
static seq_t binomial_safe(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;

    seq_t result = 1;
    for (idx_t i = 1; i <= k; ++i) {
        idx_t num   = n - k + i;            /* numerator part */
        seq_t num_s = (seq_t)num;
        seq_t den_s = (seq_t)i;             /* denominator part */

        /* cancel common factors with the current result */
        seq_t g = gcd(result, den_s);
        result /= g;
        den_s /= g;

        /* cancel remaining common factors between numerator and denominator */
        g = gcd(num_s, den_s);
        num_s /= g;
        den_s /= g;

        result *= num_s;                    /* multiply reduced numerator */

        /* if any factor remains in the denominator, divide */
        if (den_s != 1)
            result /= den_s;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The definition gives a(0) = 1; handling it separately avoids
     * a negative (wrap‑around) argument to binomial_safe. */
    if (n == 0) return 1;

    seq_t sum = 0;
    idx_t max_k = n / 3;

    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t a_param = 5 * n - 3 * k - 1;
        idx_t b_param = n - 3 * k;
        sum += binomial_safe(a_param, b_param);
    }
    return sum;
}
