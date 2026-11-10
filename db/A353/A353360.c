/*
 * A353360  a(n) = A006047(A048675(n)).
 * https://oeis.org/A353360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006047(A048675(n)), i.e. a(n) = 3^s * 2^r where
 *            A048675(n) = Sum_{p^e || n} e*2^(PrimePi(p)-1) has r ones and s
 *            twos in base 3. A048675(n) is built exactly as a base-3 digit
 *            array so no intermediate overflows
 * Range:     u8 n <= 36, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"

static uint64_t A353360_prime_index(uint64_t p)
{
    uint64_t cnt = 0;
    for (uint64_t q = 2; q <= p; ++q) {
        bool isp = true;
        for (uint64_t d = 2; d * d <= q; ++d) {
            if (q % d == 0) { isp = false; break; }
        }
        if (isp) cnt++;
    }
    return cnt;
}

/* acc += src (base-3 digit arrays, little-endian, length len) */
static void A353360_add3(unsigned char *acc, const unsigned char *src, size_t len)
{
    unsigned carry = 0;
    for (size_t i = 0; i < len; ++i) {
        unsigned s = (unsigned)acc[i] + (unsigned)src[i] + carry;
        acc[i] = (unsigned char)(s % 3);
        carry = s / 3;
    }
}

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    seq_t result = 1;
    if (n <= 1) return result;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    uint64_t maxk = 0;
    uint64_t idxs[64];
    for (size_t i = 0; i < cnt; ++i) {
        idxs[i] = A353360_prime_index(primes[i]);
        if (idxs[i] > maxk) maxk = idxs[i];
    }

    size_t len = (size_t)maxk + 16;
    unsigned char *acc = (unsigned char *)calloc(len, 1);
    unsigned char *pw = (unsigned char *)calloc(len, 1);

    for (size_t i = 0; i < cnt; ++i) {
        /* pw = 2^(idx-1) in base 3 */
        memset(pw, 0, len);
        pw[0] = 1;
        for (uint64_t j = 1; j < idxs[i]; ++j) {
            unsigned carry = 0;
            for (size_t t = 0; t < len; ++t) {
                unsigned s = (unsigned)pw[t] * 2u + carry;
                pw[t] = (unsigned char)(s % 3);
                carry = s / 3;
            }
        }
        for (uint64_t e = 0; e < exps[i]; ++e)
            A353360_add3(acc, pw, len);
    }

    for (size_t t = 0; t < len; ++t) {
        if (acc[t] == 1) result = result * (seq_t)2;
        else if (acc[t] == 2) result = result * (seq_t)3;
    }

    free(acc);
    free(pw);
    return result;
}
