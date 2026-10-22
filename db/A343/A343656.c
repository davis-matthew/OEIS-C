/*
 * A343656  Array read by antidiagonals where A(n,k) is the number of divisors
 *          of n^k.
 * https://oeis.org/A343656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 215, u16 n <= 2444, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A343656: Array read by antidiagonals where A(n,k) is the
 * number of divisors of n^k.  The term a(idx) is obtained by
 *   1. Find the diagonal d such that (d-1)*d/2 < idx ≤ d*(d+1)/2.
 *   2. Let n = idx - (d-1)*d/2          // row index (≥1)
 *   3. Let k = d - n                    // column index (≥0)
 *   4. Return d(n^k) = Π_i (k*e_i + 1)   // e_i are exponents in factorisation of n
 *
 * The code works for any valid value of idx ≥ 1 and uses only integer
 * arithmetic.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t idx) {
    /* Step 1: locate diagonal d */
    idx_t d = 1;
    /* cumulative terms up to diagonal d: d*(d+1)/2 */
    while ( (d * (d + 1ULL)) / 2ULL < idx ) {
        d++;
    }

    /* Step 2: compute n (row) and k (column) for this idx */
    idx_t prev = (d - 1) * d / 2ULL;    /* terms before diagonal d */
    idx_t n = idx - prev;              /* 1‑based within diagonal d */
    idx_t k = d - n;                    /* k ≥ 0 */

    /* Step 3: divisor count of n^k */
    if (k == 0) {
        return (seq_t)1;                /* n^0 = 1 has one divisor */
    }

    seq_t result = (seq_t)1;
    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        idx_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            exp++;
        }
        if (exp > 0) {
            seq_t term = (seq_t)k * (seq_t)exp + 1;
            result *= term;
        }
    }
    if (temp > 1) {                     /* remaining prime factor */
        seq_t term = (seq_t)k * (seq_t)1 + 1;
        result *= term;
    }
    return result;
}
