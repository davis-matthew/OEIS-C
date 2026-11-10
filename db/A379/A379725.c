/*
 * A379725  a(n) = A023900(n*A000396(n)).
 * https://oeis.org/A379725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A023900(n*A000396(n)); with A000396(n) = 2^(p-1)*M, M =
 *            2^p-1 prime, a(n) = (M-1) * Product_{odd primes q | n, q != M}
 *            (1-q), since A023900 depends only on the distinct primes
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 9, i128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 12 (2026-11-10)
 * Helpers:   is_prime_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime_u32.h"

static int pn_lucas_lehmer(unsigned p)
{
    size_t L = (p + 31) / 32, i, j;
    unsigned bits = p - 32 * (unsigned)(L - 1);
    uint32_t mask = bits == 32 ? 0xffffffffu : ((1u << bits) - 1);
    unsigned w = p / 32, b = p % 32;
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(2 * L + 1, sizeof(uint32_t));
    unsigned it;
    int res;

    s[0] = 4;
    for (it = 0; it + 2 < p; it++) {
        uint64_t carry;
        uint32_t c;
        memset(r, 0, (2 * L + 1) * sizeof(uint32_t));
        for (i = 0; i < L; i++) {
            carry = 0;
            if (s[i] == 0) continue;
            for (j = 0; j < L; j++) {
                uint64_t t = (uint64_t)r[i + j] + (uint64_t)s[i] * s[j] + carry;
                r[i + j] = (uint32_t)t;
                carry = t >> 32;
            }
            r[i + L] = (uint32_t)carry;
        }
        carry = 0;
        for (i = 0; i < L; i++) {
            uint32_t lo = r[i];
            uint32_t hi;
            if (i == L - 1) lo &= mask;
            hi = r[w + i] >> b;
            if (b) hi |= r[w + i + 1] << (32 - b);
            if (i == L - 1) hi &= mask;
            carry += (uint64_t)lo + hi;
            s[i] = (uint32_t)carry;
            carry >>= 32;
        }
        c = s[L - 1] >> bits;
        s[L - 1] &= mask;
        for (i = 0; c && i < L; i++) {
            uint64_t t = (uint64_t)s[i] + c;
            s[i] = (uint32_t)t;
            c = (uint32_t)(t >> 32);
        }
        {
            int ge2 = s[0] >= 2;
            for (i = 1; !ge2 && i < L; i++)
                if (s[i]) ge2 = 1;
            if (ge2) {
                uint32_t bor = 2;
                for (i = 0; bor && i < L; i++) {
                    uint32_t old = s[i];
                    s[i] = old - bor;
                    bor = old < bor ? 1 : 0;
                }
            } else {
                uint32_t v = s[0];
                for (i = 0; i < L; i++) s[i] = 0xffffffffu;
                s[L - 1] = mask;
                s[0] -= (2 - v);
            }
        }
    }
    {
        int zero = 1, full = 1;
        for (i = 0; i < L; i++) {
            if (s[i]) zero = 0;
            if (s[i] != (i == L - 1 ? mask : 0xffffffffu)) full = 0;
        }
        res = zero || full;
    }
    free(s);
    free(r);
    return res;
}

static uint64_t pn_powmod2(unsigned e, uint64_t q)
{
    uint64_t r = 1 % q, b = 2 % q;
    while (e) {
        if (e & 1) r = r * b % q;
        b = b * b % q;
        e >>= 1;
    }
    return r;
}

static int pn_small_factor(unsigned p)
{
    uint64_t k;
    for (k = 1; k <= 4000; k++) {
        uint64_t q = 2 * k * (uint64_t)p + 1;
        if (q >= ((uint64_t)1 << 32)) break;
        if (p < 63 && q >= (((uint64_t)1 << p) - 1)) break;
        if (q % 8 != 1 && q % 8 != 7) continue;
        if (pn_powmod2(p, q) == 1) return 1;
    }
    return 0;
}

static int pn_width(void)
{
    seq_t x = 1;
    int k = 0;
    while (k < 300) {
        x <<= 1;
        k++;
        if (x == 0) return k;
    }
    return 0;
}

/* n-th Mersenne prime exponent, or 0 if 2^p-1 does not fit in seq_t */
static unsigned pn_exponent(idx_t n)
{
    static unsigned ex[256];
    static idx_t cnt = 0;
    static unsigned next_p = 2;
    static int width = -1;
    unsigned p;

    if (n < 1 || n > 256) return 0;
    if (width < 0) width = pn_width();

    while (cnt < n) {
        p = next_p;
        if (width > 0 && p + 1 > (unsigned)width) return 0;
        next_p++;
        if (!is_prime_u32(p)) continue;
        if (p == 2 || (!pn_small_factor(p) && pn_lucas_lehmer(p)))
            ex[cnt++] = p;
    }
    p = ex[n - 1];
    if (width > 0 && p + 1 > (unsigned)width) return 0;
    return p;
}

seq_t a(idx_t n)
{
    unsigned p = pn_exponent(n);
    seq_t M, result;
    idx_t m, q;

    if (p == 0) return 0;
    M = (seq_t)1;
    M <<= (p - 1);
    M = (M - 1) * 2 + 1;

    result = M - 1;
    m = n;
    while ((m & 1) == 0) m >>= 1;
    for (q = 3; q <= m / q; q += 2) {
        if (m % q == 0) {
            if ((seq_t)q != M)
                result *= ((seq_t)1 - (seq_t)q);
            while (m % q == 0) m /= q;
        }
    }
    if (m > 1 && (seq_t)m != M)
        result *= ((seq_t)1 - (seq_t)m);
    return result;
}
