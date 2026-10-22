/*
 * A359833  Dirichlet inverse of A359832, where A359832(n) = 1 if the 2-adic
 *          valuation of n is either 0 or odd, otherwise 0.
 * https://oeis.org/A359833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is the Dirichlet inverse of A359832. Multiplicative with:
 *            a(2^e) = (-1)^e * Fibonacci(e) a(p^e) = -1 if e = 1 and p>2,
 *            else 0
 * Range:     i8 n <= 4095, i16 n <= 9999, i32 n <= 9999, i64 n <= 9999,
 *            i128 n <= 9999, big any n
 * Helpers:   factor_u64, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/fib.h"

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;          /* base case */
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt); /* factor n */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t val;
        if (p == 2) {
            val = fib((idx_t)e);          /* Fibonacci(e) */
            if (e & 1) val = -val;        /* multiply by (-1)^e */
        } else {
            if (e == 1) {
                val = -1;
            } else {
                return (seq_t)0;           /* any higher power of odd prime gives 0 */
            }
        }
        result *= val;
        if (result == 0) return result;   /* early exit if zero */
    }
    return result;
}
