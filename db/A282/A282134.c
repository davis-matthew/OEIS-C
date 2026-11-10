/*
 * A282134  First differences of A045876.
 * https://oeis.org/A282134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = A045876(n+1) - A045876(n), where A045876(m) =
 *            ((10^k-1)/9) * (digitsum(m)*N(m)/k), k = number of digits of m,
 *            N(m) = number of distinct permutations of the digits of m
 * Range:     i8 n <= 100, i16 n <= 1028, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9511 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A045876_a(idx_t n)
{
    unsigned int freq[10] = {0};
    seq_t sum_digits = 0;
    idx_t dcnt = 0;
    idx_t temp = n;
    do {
        unsigned int d = (unsigned int)(temp % 10);
        freq[d]++;
        sum_digits += (seq_t)d;
        temp /= 10;
        dcnt++;
    } while (temp > 0);

    /* multinomial coefficient built multiplicatively */
    seq_t N = 1;
    seq_t t = 0;
    for (int d = 0; d < 10; ++d) {
        for (unsigned int i = 1; i <= freq[d]; ++i) {
            t++;
            N = N * t / (seq_t)i;
        }
    }

    seq_t I = 0;
    for (idx_t i = 0; i < dcnt; ++i)
        I = I * 10 + 1;

    seq_t s = sum_digits * N / (seq_t)dcnt;
    return s * I;
}

seq_t a(idx_t n)
{
    return A045876_a(n + 1) - A045876_a(n);
}
