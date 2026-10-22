/*
 * A349272  a(n) = Product_{k = 1..2*n+1} Fibonacci(2*k) / Sum_{k = 1..2*n+1}
 *          Fibonacci(2*k).
 * https://oeis.org/A349272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{k=1..2n+1} Fibonacci(2k) / sum_{k=1..2n+1}
 *            Fibonacci(2k) * Equivalent integer form: a(n) =
 *            Fibonacci(2n+1)/Fibonacci(2n+2) * product_{k=1..2n}
 *            Fibonacci(2k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   fib, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/fib.h"
#include "../utils/gcd.h"

static seq_t *a_cache = NULL;   /* memoised a(n) values */
static idx_t  a_len   = 0;      /* number of cached entries   */
static size_t a_cap   = 0;      /* allocated capacity         */

seq_t a(idx_t n)
{
    /* initialise cache for a(0) */
    if (a_len == 0) {
        a_cap = 1;
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = (seq_t)1;
        a_len = 1;
    }

    /* grow capacity if needed */
    if (n >= a_len) {
        size_t new_cap = a_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache = tmp;
        a_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = a_len; i <= n; ++i) {
        if (i == 0) {
            a_cache[0] = (seq_t)1;
            continue;
        }

        /* Prepare factors for recurrence:
         * a(i) = a(i-1) *
         *        [F(4i-2)*F(4i)*F(2i+1)*F(2i)] /
         *        [F(2i-1)*F(2i+2)]           */
        seq_t num[4];
        seq_t den[2];

        num[0] = fib(4 * i - 2); /* F(4i-2) */
        num[1] = fib(4 * i);     /* F(4i)   */
        num[2] = fib(2 * i + 1); /* F(2i+1) */
        num[3] = fib(2 * i);     /* F(2i)   */

        den[0] = fib(2 * i - 1); /* F(2i-1) */
        den[1] = fib(2 * i + 2); /* F(2i+2) */

        seq_t res = a_cache[i - 1];

        /* Cancel common factors between res and denominators */
        for (int j = 0; j < 2; ++j) {
            seq_t g = gcd(res, den[j]);
            if (g > 1) {
                res /= g;
                den[j] /= g;
            }
        }

        /* Cancel remaining denominator factors with numerator factors */
        for (int k = 0; k < 4; ++k) {
            for (int j = 0; j < 2; ++j) {
                if (den[j] == 1) continue;
                seq_t g = gcd(num[k], den[j]);
                if (g > 1) {
                    num[k] /= g;
                    den[j] /= g;
                }
            }
        }

        /* Multiply the remaining numerator factors */
        for (int k = 0; k < 4; ++k) {
            res *= num[k];
        }

        /* Divide by any remaining denominator factors (should be 1) */
        for (int j = 0; j < 2; ++j) {
            if (den[j] > 1)
                res /= den[j];
        }

        a_cache[i] = res;
    }

    a_len = n + 1;
    return a_cache[n];
}
