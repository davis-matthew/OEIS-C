/*
 * A256154  Concatenation of odd prime factors of m such that the decimal
 *          digits of m only have odd prime factors.
 * https://oeis.org/A256154
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of the odd prime factors (with
 *            multiplicity) of the nth positive integer whose decimal digits
 *            are only 3, 5, 7, or 9 (digits 1 or even digits are not
 *            allowed). The factors are listed in ascending order, each
 *            occurrence repeated. For each prime factor p, the concatenation
 *            appe...
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   digit_len, factor_u64, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

static const int digit_map[4] = {3, 5, 7, 9};

seq_t a(idx_t n) {
    /* Handle offset (n >= 1 by definition, but guard just in case) */
    if (n < 1) return 0;

    /* 1. Find the length L of the m(n) number and its zero‑based offset */
    idx_t cum = 0;         /* sum of 4^k for k < L */
    idx_t pow4 = 4;        /* 4^1 */
    idx_t len = 1;         /* current length */

    while (cum + pow4 < n) {
        cum += pow4;
        pow4 <<= 2;        /* multiply by 4 */
        len++;
    }

    idx_t offset = n - cum - 1;          /* zero‑based index within length L */

    /* 2. Decode offset into base‑4 digits of length `len` */
    uint64_t temp = (uint64_t)offset;
    int digits[16];                       /* len <= 7 for n <= 10000 */
    for (int i = (int)len - 1; i >= 0; --i) {
        digits[i] = (int)(temp % 4);
        temp /= 4;
    }

    /* 3. Build the decimal number m from the digit map */
    uint64_t m_val = 0;
    for (int i = 0; i < (int)len; ++i) {
        m_val = m_val * 10 + (uint64_t)digit_map[digits[i]];
    }

    /* 4. Factor m_val into prime powers (all primes are odd) */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(m_val, primes, exps, &cnt);

    /* 5. Create an array of all prime factors, repeated according to exponents */
    uint64_t factors[64];
    size_t fac_total = 0;
    for (size_t i = 0; i < cnt; ++i) {
        for (uint64_t k = 0; k < exps[i]; ++k) {
            factors[fac_total++] = primes[i];
        }
    }

    /* 6. Concatenate the prime factors into the result value */
    seq_t result = 0;
    for (size_t i = 0; i < fac_total; ++i) {
        seq_t f = (seq_t)factors[i];
        int fib_len = digit_len(f);
        seq_t pow = pow10((idx_t)fib_len);
        result = result * pow + f;
    }

    return result;
}
