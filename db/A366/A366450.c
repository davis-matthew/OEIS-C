/*
 * A366450  a(n) = Sum_{k=1..n} A366362(n,k)*A023900(k)/n.
 * https://oeis.org/A366450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} A366362(n,k)*A023900(k)/n, i.e. a(n) =
 *            (1/n)*Sum_{x=1..n} Sum_{y=1..n} A023900(GCD(x^3-x^2-y^2-y, n)),
 *            where A023900(m) = Product_{p|m} (1-p)
 * Range:     i8 n <= 15, i16 n <= 84, i32 n <= 84, i64 n <= 84, i128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static uint64_t a366450_gcd(uint64_t a, uint64_t b) {
    while (b) {
        uint64_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static seq_t a366450_A023900(uint64_t m) {
    seq_t result = (seq_t)1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            result *= ((seq_t)1 - (seq_t)p);
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)
        result *= ((seq_t)1 - (seq_t)m);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t *cnt = (uint64_t *)calloc((size_t)N + 1, sizeof(uint64_t));
    if (cnt == NULL)
        abort();

    for (uint64_t x = 1; x <= N; x++) {
        long long xx = (long long)x;
        long long base = xx * xx * xx - xx * xx;
        for (uint64_t y = 1; y <= N; y++) {
            long long yy = (long long)y;
            long long f = base - yy * yy - yy;
            long long m = f % (long long)N;
            if (m < 0) m += (long long)N;
            uint64_t g = a366450_gcd((uint64_t)m, N); /* gcd(0,N) = N */
            cnt[g]++;
        }
    }

    seq_t sum = (seq_t)0;
    for (uint64_t k = 1; k <= N; k++) {
        if (cnt[k])
            sum += (seq_t)cnt[k] * a366450_A023900(k);
    }
    free(cnt);
    return sum / (seq_t)N;
}
