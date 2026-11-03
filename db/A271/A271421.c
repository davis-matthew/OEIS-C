/*
 * A271421  a(n) = fibonorial(3*n)/(fibonorial(2*n+1)*fibonorial(n+1)), where
 *          fibonorial(n) = A003266(n).
 * https://oeis.org/A271421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fibonorial(3*n)/(fibonorial(2*n+1)*fibonorial(n+1))
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* a(0) is outside the offset; return 0 for completeness */
    if (n == 0)
        return 0;

    /* Allocate array of denominator factors fib(1)..fib(n+1) */
    const idx_t den_count = n + 1;
    seq_t *denoms = mem_new(seq_t, den_count);
    if (!denoms)          /* out of memory – return 0 as a safe fallback */
        return 0;

    for (idx_t i = 0; i < den_count; ++i)
        denoms[i] = fib(i + 1);           /* fib(1) .. fib(n+1) */

    seq_t result = (seq_t)1;
    idx_t solved = 0;                      /* how many denominator factors are 1 */

    /* Multiply numerator factors fib(2n+2)..fib(3n) and cancel as we go */
    for (idx_t j = 2 * n + 2; j <= 3 * n; ++j) {
        result *= fib(j);

        /* Only try to cancel if we still have unreduced denominators */
        if (solved < den_count) {
            for (idx_t k = 0; k < den_count; ++k) {
                seq_t g = gcd(result, denoms[k]);
                if (g != 1) {
                    result /= g;
                    denoms[k] /= g;
                    if (denoms[k] == 1)
                        ++solved;
                    if (solved == den_count)
                        break;           /* all denominators cancelled */
                }
            }
        }
    }

    /* Final cancellation pass – just in case any denominator remains */
    if (solved < den_count) {
        for (idx_t k = 0; k < den_count; ++k) {
            if (denoms[k] != 1) {
                seq_t g = gcd(result, denoms[k]);
                if (g != 1) {
                    result /= g;
                    denoms[k] /= g;
                }
            }
        }
    }

    mem_free(denoms);
    return result;
}
