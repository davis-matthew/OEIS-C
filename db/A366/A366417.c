/*
 * A366417  a(n) = A006571(A005117(n)).
 * https://oeis.org/A366417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A006571(A005117(n)); A005117(n) is the n-th squarefree
 *            number, found by search. A006571 is multiplicative with a(11)=1
 *            and a(p) = p - #{(x,y) in F_p^2 : y^2+y = x^3-x^2}. Since the
 *            argument is squarefree, a(m) is the product of a(p) over primes
 *            p dividing m
 * Range:     i8 n <= 73, i16 n <= 73, i32 n <= 73, i64 n <= 73, i128 n <= 73,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 73 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static bool A005117_sqfree(uint64_t k) {
    if (k % 4 == 0) return false;
    for (uint64_t i = 3; i * i <= k; i += 2)
        if (k % (i * i) == 0) return false;
    return true;
}

static long long A006571_ap(uint64_t p) {
    if (p == 11) return 1;
    unsigned char *cnt = (unsigned char *)calloc((size_t)p, 1);
    for (uint64_t y = 0; y < p; y++) cnt[(y * y + y) % p]++;
    long long N = 0;
    for (uint64_t x = 0; x < p; x++) {
        uint64_t x2 = (x * x) % p;
        uint64_t x3 = (x2 * x) % p;
        N += cnt[(x3 + p - x2) % p];
    }
    free(cnt);
    return (long long)p - N;
}

seq_t a(idx_t n) {
    uint64_t k = 0;
    for (idx_t i = 0; i < n; i++) {
        do { k++; } while (!A005117_sqfree(k));
    }
    seq_t res = 1;
    uint64_t m = k;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            res *= (seq_t)A006571_ap(p);
            if (res == 0) return res;
        }
    }
    if (m > 1) res *= (seq_t)A006571_ap(m);
    return res;
}
