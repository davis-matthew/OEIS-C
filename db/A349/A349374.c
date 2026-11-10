/*
 * A349374  Dirichlet convolution of Kimberling's paraphrase, A003602, with
 *          squarefree part of n (A007913).
 * https://oeis.org/A349374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A003602(n/d) * A007913(d)
 * Range:     u8 n <= 65, u16 n <= 5609, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 92 terms through n = 9990 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A003602_idx(idx_t n) {
    while ((n & 1) == 0) n >>= 1;
    return (n + 1) >> 1;
}

static idx_t A007913_idx(idx_t n) {
    idx_t result = 1;
    idx_t temp = n;
    idx_t i;
    for (i = 2; i <= temp / i; i++) {
        int e = 0;
        while (temp % i == 0) {
            temp /= i;
            e++;
        }
        if (e & 1) result *= i;
    }
    if (temp > 1) result *= temp;
    return result;
}

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d <= n / d; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            sum += (seq_t)A003602_idx(e) * (seq_t)A007913_idx(d);
            if (e != d) {
                sum += (seq_t)A003602_idx(d) * (seq_t)A007913_idx(e);
            }
        }
    }
    return sum;
}
