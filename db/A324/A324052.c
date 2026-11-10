/*
 * A324052  a(n) = A083254(A005940(1+n)).
 * https://oeis.org/A324052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A083254(A005940(1+n)) = 2*phi(m) - m, where m =
 *            A005940(1+n), computed from its prime factorization
 * Range:     i8 n <= 55, i16 n <= 471, i32 n <= 8191, i64 n <= 8191,
 *            i128 n <= 8191, big any n
 * Validated: standard, pass, 99 terms through n = 7382 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void A324052_primes(uint64_t *pr, int cnt)
{
    int k = 0;
    uint64_t c = 2;
    while (k < cnt) {
        int ok = 1;
        for (int j = 0; j < k; j++) {
            if (pr[j] * pr[j] > c) break;
            if (c % pr[j] == 0) { ok = 0; break; }
        }
        if (ok) pr[k++] = c;
        c++;
    }
}

seq_t a(idx_t n)
{
    uint64_t pr[72];
    int ex[72];
    A324052_primes(pr, 72);
    for (int i = 0; i < 72; i++) ex[i] = 0;

    idx_t m = n;
    int i = 0;
    while (m > 0) {
        if (m & 1) {
            ex[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }

    seq_t M = 1, phi = 1;
    for (int k = 0; k < 72; k++) {
        if (ex[k] == 0) continue;
        for (int e = 0; e < ex[k]; e++) {
            M *= (seq_t)pr[k];
            if (e == 0) phi *= (seq_t)(pr[k] - 1);
            else phi *= (seq_t)pr[k];
        }
    }
    return 2 * phi - M;
}
