/*
 * A391565  Denominator of the constant term of the remainder obtained when
 *          Stern polynomial B(n,x) is divided by B(5,x).
 * https://oeis.org/A391565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of B_n(-1/2) in lowest terms, where B_n(x) is
 *            the Stern polynomial defined by B_0 = 0, B_1 = 1, B_{2n} = x *
 *            B_n, B_{2n+1} = B_n + B_{n+1}
 * Range:     u8 n <= 255, u16 n <= 9546, u32 n <= 9546, u64 n <= 9546,
 *            u128 n <= 9546, big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses the recurrence
   f_0 = 0, f_1 = 1,
   f_{2n}   = -f_n / 2,
   f_{2n+1} = f_n + f_{n+1},
   where f_n = B_n(-1/2).  The result a(n) is the
   denominator of the reduced fraction f_n.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/pow_seq.h"

static seq_t *num_cache = NULL;   /* numerator of f_n */
static idx_t *exp_cache = NULL;   /* exponent of the denominator 2^exp */
static bool   *computed = NULL;   /* flag whether f_n is known */
static size_t cache_size = 0;     /* allocated size */

/* Ensure the cache can hold values up to idx */
static void
ensure_size(idx_t idx)
{
    size_t new_size = idx + 1;
    if (cache_size < new_size) {
        num_cache   = (seq_t *)realloc(num_cache,  new_size * sizeof(seq_t));
        exp_cache   = (idx_t *)realloc(exp_cache,  new_size * sizeof(idx_t));
        computed    = (bool  *)realloc(computed,   new_size * sizeof(bool));
        for (size_t i = cache_size; i < new_size; ++i)
            computed[i] = false;
        cache_size = new_size;
    }
}

/* Recursively compute f_n as a reduced fraction num/2^exp */
static void
compute_frac(idx_t n)
{
    if (computed[n])
        return;

    if (n == 0) {
        num_cache[n] = (seq_t)0;
        exp_cache[n] = 0;
        computed[n]  = true;
        return;
    }
    if (n == 1) {
        num_cache[n] = (seq_t)1;
        exp_cache[n] = 0;
        computed[n]  = true;
        return;
    }

    if ((n & 1) == 0) {                     /* n is even */
        idx_t half = n >> 1;
        compute_frac(half);
        num_cache[n] = -num_cache[half];
        exp_cache[n] = exp_cache[half] + 1;
    } else {                                 /* n is odd, n = 2*half + 1 */
        idx_t half = n >> 1;                 /* floor(n/2) */
        idx_t ceil = half + 1;               /* ceil(n/2)  = n - half */
        compute_frac(half);
        compute_frac(ceil);

        idx_t exp_a = exp_cache[half];
        idx_t exp_b = exp_cache[ceil];
        idx_t exp_c = exp_a > exp_b ? exp_a : exp_b;

        /* Align exponents */
        idx_t shift_a = exp_c - exp_a;
        idx_t shift_b = exp_c - exp_b;

        seq_t part_a = num_cache[half];
        seq_t part_b = num_cache[ceil];

        part_a = part_a << shift_a;
        part_b = part_b << shift_b;

        num_cache[n] = part_a + part_b;
        exp_cache[n] = exp_c;
    }

    /* Reduce the fraction: cancel powers of 2 */
    if (num_cache[n] == (seq_t)0) {
        exp_cache[n] = 0;    /* zero has denominator 1 */
    } else {
        while (exp_cache[n] > 0 && ((num_cache[n] & (seq_t)1) == (seq_t)0)) {
            num_cache[n] = num_cache[n] >> 1;
            exp_cache[n]--;
        }
    }
    computed[n] = true;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, return 0 */

    ensure_size(n);
    compute_frac(n);

    if (num_cache[n] == (seq_t)0)
        return (seq_t)1;          /* denominator of 0 is 1 */

    return pow_seq((seq_t)2, (int)exp_cache[n]);
}
