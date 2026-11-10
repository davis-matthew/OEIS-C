/*
 * A300723  Möbius-transform of A005187(A053645(n)).
 * https://oeis.org/A300723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*A005187(A053645(d)), where A053645(d) =
 *            d - 2^floor(log2(d)) and A005187(m) = Sum_{k>=0} floor(m/2^k)
 * Range:     i8 n <= 192, i16 n <= 16384, i32 n <= 16384, i64 n <= 16384,
 *            i128 n <= 16384, big any n
 * Validated: standard, pass, 106 terms through n = 8772 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int mu_local(uint64_t m) {
    int cnt = 0;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            m /= p;
            if (m % p == 0) return 0;
            ++cnt;
        }
    }
    if (m > 1) ++cnt;
    return (cnt & 1) ? -1 : 1;
}

static seq_t f_local(uint64_t d) {
    uint64_t top = d;
    uint64_t pw = 1;
    while (top >>= 1) pw <<= 1;
    uint64_t m = d - pw;
    seq_t s = 0;
    while (m > 0) {
        s += (seq_t)m;
        m >>= 1;
    }
    return s;
}

static void add_term(uint64_t n, uint64_t d, seq_t *sum) {
    int mu = mu_local(n / d);
    if (mu == 0) return;
    seq_t v = f_local(d);
    if (mu > 0) *sum += v; else *sum -= v;
}

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    seq_t sum = 0;
    if (N == 0) return 0;
    for (uint64_t i = 1; i <= N / i; ++i) {
        if (N % i == 0) {
            add_term(N, i, &sum);
            uint64_t j = N / i;
            if (j != i) add_term(N, j, &sum);
        }
    }
    return sum;
}
