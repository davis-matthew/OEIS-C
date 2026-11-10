/*
 * A349621  Dirichlet convolution of A003415 with the Dirichlet inverse of
 *          A003959.
 * https://oeis.org/A349621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A003415(n/d) * A063441(d), A063441(n) =
 *            sigma(n)*mu(n)
 * Range:     i8 n <= 255, i16 n <= 16383, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 104 terms through n = 8841 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A003415_a(idx_t n) {
    if (n <= 1) return 0;
    seq_t result = 0;
    idx_t rem = n;
    for (idx_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            result += (seq_t)(n / p) * (seq_t)e;
        }
    }
    if (rem > 1) result += (seq_t)(n / rem);
    return result;
}

static seq_t A063441_a(idx_t n) {
    if (n == 1) return 1;
    seq_t sigma = 1;
    int cnt = 0;
    idx_t rem = n;
    for (idx_t p = 2; p * p <= rem; p++) {
        if (rem % p == 0) {
            int e = 0;
            while (rem % p == 0) { rem /= p; e++; }
            if (e > 1) return 0;
            sigma *= (seq_t)(p + 1);
            cnt++;
        }
    }
    if (rem > 1) {
        sigma *= (seq_t)(rem + 1);
        cnt++;
    }
    return (cnt & 1) ? -sigma : sigma;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            idx_t j = n / i;
            total += A003415_a(j) * A063441_a(i);
            if (j != i)
                total += A003415_a(i) * A063441_a(j);
        }
    }
    return total;
}
