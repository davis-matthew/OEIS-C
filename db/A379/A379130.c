/*
 * A379130  a(n) is the number of unitary divisors d of n for which
 *          A048720(A065621(sigma(d)),sigma(n/d)) is equal to sigma(n).
 * https://oeis.org/A379130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, gcd(d,n/d)=1}
 *            [A048720(A065621(sigma(d)),sigma(n/d)) == sigma(n)]; computed by
 *            enumerating unitary divisors via factorization
 * Range:     u8 n <= 65, u16 n <= 65536, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 8628 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t A065621_f(seq_t x)
{
    seq_t low = 1;
    while ((x & low) == 0) low <<= 1;
    return x ^ ((x - low) << 1);
}

static seq_t A048720_f(seq_t i, seq_t j)
{
    seq_t result = 0;
    seq_t shifted = i;
    while (j != 0) {
        if ((j & 1) != 0) result ^= shifted;
        shifted <<= 1;
        j >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t s[24];
    int k = 0;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            seq_t sum = 1, pw = 1;
            while (m % p == 0) {
                m /= p;
                pw *= (seq_t)p;
                sum += pw;
            }
            s[k++] = sum;
        }
    }
    if (m > 1) {
        s[k++] = (seq_t)1 + (seq_t)m;
    }
    seq_t total = 1;
    for (int i = 0; i < k; i++) total *= s[i];

    seq_t count = 0;
    for (unsigned mask = 0; mask < (1u << k); mask++) {
        seq_t sd = 1, sr = 1;
        for (int i = 0; i < k; i++) {
            if ((mask >> i) & 1u) sd *= s[i];
            else sr *= s[i];
        }
        if (A048720_f(A065621_f(sd), sr) == total) count++;
    }
    return count;
}
