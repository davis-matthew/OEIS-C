/*
 * A347235  Dirichlet convolution of Euler phi with A342001, where A342001(n)
 *          = A003415(n) / A003557(n).
 * https://oeis.org/A347235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * A342001(d), where A342001(d) =
 *            Sum_{p^v || d} v*rad(d)/p; divisors enumerated from the
 *            factorization of n
 * Range:     u8 n <= 83, u16 n <= 4619, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 101 terms through n = 8336 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    idx_t pr[64];
    int ex[64];
    int k = 0;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; e++; }
            pr[k] = p; ex[k] = e; k++;
        }
    }
    if (m > 1) { pr[k] = m; ex[k] = 1; k++; }

    int cur[64];
    for (int i = 0; i < k; i++) cur[i] = 0;

    seq_t total = 0;
    for (;;) {
        /* phi(n/d) and f(d) for current exponent vector */
        seq_t ph = 1;
        seq_t rad = 1;
        for (int i = 0; i < k; i++) {
            int b = ex[i] - cur[i];
            if (b > 0) {
                ph *= (seq_t)(pr[i] - 1);
                for (int j = 1; j < b; j++) ph *= (seq_t)pr[i];
            }
            if (cur[i] > 0) rad *= (seq_t)pr[i];
        }
        seq_t f = 0;
        for (int i = 0; i < k; i++) {
            if (cur[i] > 0)
                f += (seq_t)cur[i] * (rad / (seq_t)pr[i]);
        }
        total += ph * f;

        int i = 0;
        while (i < k) {
            if (cur[i] < ex[i]) { cur[i]++; break; }
            cur[i] = 0;
            i++;
        }
        if (i == k) break;
    }
    return total;
}
