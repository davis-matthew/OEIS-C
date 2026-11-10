/*
 * A351449  a(n) = A064989(A295294(A003961(n))).
 * https://oeis.org/A351449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = A064989(A295294(A003961(n))); multiplicative with a(p)=1,
 *            a(p^e)=A064989((q^(e+1)-1)/(q-1)) for e>1, q=nextPrime(p)
 * Range:     u8 n <= 31, u16 n <= 1023, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 105 terms through n = 9617 (2026-11-10)
 * Helpers:   factor_u64, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/next_prime.h"

static uint64_t A351449_prev_prime(uint64_t n)
{
    uint64_t p = n - 1;
    while (p > 2 && !is_prime_u64(p)) --p;
    return p;
}

static seq_t A351449_term(uint64_t r, uint64_t f)
{
    seq_t res = 1;
    if (r == 2) return res;
    seq_t base = (seq_t)A351449_prev_prime(r);
    for (uint64_t i = 0; i < f; ++i) res *= base;
    return res;
}

/* A064989 of s (seq_t) */
static seq_t A351449_A064989(seq_t s)
{
    seq_t result = 1;
    bool fits = (sizeof(seq_t) <= 8) || !((seq_t)UINT64_MAX < s);
    if (fits) {
        uint64_t pr[64];
        uint64_t ex[64];
        size_t cnt = 0;
        factor_u64((uint64_t)s, pr, ex, &cnt);
        for (size_t i = 0; i < cnt; ++i)
            result *= A351449_term(pr[i], ex[i]);
        return result;
    }
    /* large: trial division */
    seq_t m = s;
    for (seq_t d = 2; d <= m / d; d += (d == 2 ? 1 : 2)) {
        if (m % d == 0) {
            uint64_t f = 0;
            while (m % d == 0) { m /= d; ++f; }
            result *= A351449_term((uint64_t)d, f);
        }
    }
    if (m > 1) result *= A351449_term((uint64_t)m, 1);
    return result;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    if (m < 1) return 0;
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; ++e; }
            if (e >= 2) {
                seq_t q = (seq_t)next_prime(p);
                seq_t s = 1, pw = 1;
                for (uint64_t i = 0; i < e; ++i) {
                    pw *= q;
                    s += pw;
                }
                result *= A351449_A064989(s);
            }
        }
    }
    return result;
}
