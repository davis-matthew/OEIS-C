/*
 * A324114  A000120-deficiency of n permuted by Doudna-sequence: a(n) =
 *          A192895(A005940(1+n)).
 * https://oeis.org/A324114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A192895(A005940(1+n)); A005940(1+n) = product of
 *            prime(k)^(number of 1's in binary n followed by k-1 zeros);
 *            A192895(x) = Sum_{d|x, d<x} popcount(d) - popcount(x)
 * Range:     i8 n <= 43, i16 n <= 1635, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 115 terms through n = 9800 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

static unsigned long long A324114_kth_prime(int k)
{
    unsigned long long p = 1;
    int cnt = 0;
    while (cnt < k) {
        p++;
        bool ok = true;
        for (unsigned long long q = 2; q * q <= p; q++) {
            if (p % q == 0) { ok = false; break; }
        }
        if (ok) cnt++;
    }
    return p;
}

static int A324114_popcnt(seq_t x)
{
    int c = 0;
    while (x > 0) {
        if ((x & 1) != 0) c++;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    int ex[70];
    for (int i = 0; i < 70; i++) ex[i] = 0;

    idx_t m = n;
    int i = 1;
    while (m > 0) {
        if (m & 1) {
            ex[i]++;
            m = (m - 1) / 2;
        } else {
            m /= 2;
            i++;
        }
    }

    size_t total = 1;
    seq_t x = 1;
    for (int k = 1; k < 70; k++) {
        if (ex[k] > 0) {
            total *= (size_t)(ex[k] + 1);
        }
    }

    seq_t *d = mem_new(seq_t, total);
    d[0] = 1;
    size_t cur = 1;
    for (int k = 1; k < 70; k++) {
        if (ex[k] == 0) continue;
        seq_t p = (seq_t)A324114_kth_prime(k);
        size_t base = cur;
        for (int e = 1; e <= ex[k]; e++) {
            for (size_t j = 0; j < base; j++) {
                d[(size_t)e * base + j] = d[(size_t)(e - 1) * base + j] * p;
            }
            x *= p;
        }
        cur = base * (size_t)(ex[k] + 1);
    }

    seq_t sum = 0;
    for (size_t j = 0; j < cur; j++) {
        sum += (seq_t)A324114_popcnt(d[j]);
    }
    sum -= (seq_t)(2 * A324114_popcnt(x));
    mem_free(d);
    return sum;
}
