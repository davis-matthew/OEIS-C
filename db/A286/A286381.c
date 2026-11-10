/*
 * A286381  Compound filter: a(n) = P(A055881(n), A278236(n)), where P(n,k) is
 *          sequence A000027 used as a pairing function.
 * https://oeis.org/A286381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2 + (A055881(n)+A278236(n))^2 - A055881(n) -
 *            3*A278236(n))/2
 * Range:     u8 n <= 8, u16 n <= 34, u32 n <= 190, u64 n <= 2038,
 *            u128 n <= 10080, big any n
 * Validated: standard, pass, 81 terms through n = 9600 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static seq_t A055881_a(idx_t n)
{
    idx_t factorial = 1;
    seq_t result = 0;
    idx_t m = 1;
    while (1) {
        if (factorial > n / m) break;
        factorial *= m;
        if (n % factorial == 0) {
            result = (seq_t)m;
        } else {
            break;
        }
        ++m;
    }
    return result;
}

static const unsigned A278236_p[] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71
};

static seq_t A278236_a(idx_t n)
{
    uint8_t exps[24];
    size_t cnt = 0;
    idx_t m = n;
    size_t pos = 0;
    while (m > 0) {
        uint64_t digit = m % (pos + 2);
        if (digit != 0) {
            exps[cnt++] = (uint8_t)digit;
        }
        m /= (pos + 2);
        pos++;
    }
    for (size_t i = 0; i < cnt; ++i)
        for (size_t j = i + 1; j < cnt; ++j)
            if (exps[i] < exps[j]) {
                uint8_t t = exps[i];
                exps[i] = exps[j];
                exps[j] = t;
            }
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t prime = (seq_t)A278236_p[i];
        for (uint8_t k = 0; k < exps[i]; ++k)
            result *= prime;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t x = A055881_a(n);
    seq_t y = A278236_a(n);
    seq_t s = x + y;
    return (2 + s * s - x - 3 * y) / 2;
}
