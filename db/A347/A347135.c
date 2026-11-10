/*
 * A347135  a(n) = Sum_{d|n} A001615(n/d) * A069359(d).
 * https://oeis.org/A347135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A001615(n/d) * A069359(d), A001615 = Dedekind
 *            psi, A069359(n) = n*Sum_{p|n} 1/p
 * Range:     u8 n <= 35, u16 n <= 1439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9985 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A001615_a(idx_t n) {
    if (n == 1) return 1;
    seq_t result = 1;
    idx_t temp = n;
    for (idx_t p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                e++;
                temp /= p;
            }
            seq_t contribution = (seq_t)(p + 1);
            for (idx_t i = 1; i < e; i++) contribution *= (seq_t)p;
            result *= contribution;
        }
    }
    if (temp > 1) result *= (seq_t)(temp + 1);
    return result;
}

static seq_t A069359_a(idx_t n) {
    if (n <= 1) return 0;
    seq_t result = 0;
    idx_t m = n;
    if ((m & 1) == 0) {
        result += (seq_t)(n / 2);
        while ((m & 1) == 0) m >>= 1;
    }
    for (idx_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result += (seq_t)(n / p);
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) result += (seq_t)(n / m);
    return result;
}

seq_t a(idx_t n) {
    seq_t total = 0;
    for (idx_t i = 1; i <= n / i; i++) {
        if (n % i == 0) {
            idx_t j = n / i;
            total += A001615_a(j) * A069359_a(i);
            if (i != j) total += A001615_a(i) * A069359_a(j);
        }
    }
    return total;
}
