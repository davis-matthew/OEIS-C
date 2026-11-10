/*
 * A378756  Dirichlet convolution of A046692 and A005187, where A046692 is the
 *          Dirichlet inverse of sigma.
 * https://oeis.org/A378756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{d|n} A046692(d)*A005187(n/d)
 * Range:     i8 n <= 135, i16 n <= 20000, i32 n <= 20000, i64 n <= 20000,
 *            i128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9749 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A046692_a(uint64_t n)
{
    if (n == 1) return 1;
    uint64_t m = n;
    seq_t res = 1;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int ex = 0;
            while (m % p == 0) {
                m /= p;
                ++ex;
                if (ex > 2) return 0;
            }
            if (ex == 1) {
                seq_t factor = (seq_t)(-(seq_t)p - 1);
                res *= factor;
            } else {
                seq_t factor = (seq_t)p;
                res *= factor;
            }
        }
    }
    if (m > 1) {
        seq_t factor = (seq_t)(-(seq_t)m - 1);
        res *= factor;
    }
    return res;
}

static seq_t A005187_a(uint64_t n)
{
    seq_t result = 0;
    seq_t power = (seq_t)n;
    while (power > 0) {
        result += power;
        power >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    seq_t total = 0;
    for (uint64_t i = 1; i <= N / i; ++i) {
        if (N % i == 0) {
            uint64_t j = N / i;
            total += A046692_a(i) * A005187_a(j);
            if (j != i)
                total += A046692_a(j) * A005187_a(i);
        }
    }
    return total;
}
