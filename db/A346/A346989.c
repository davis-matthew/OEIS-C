/*
 * A346989  Decimal expansion of Sum_{k >= 0} A333922(n)/2^n.
 * https://oeis.org/A346989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n-1) * S) mod 10, S = Sum_{k>=0}
 *            A333922(k)/2^k, A333922(k) = 1 -
 *            (floor(2^k*A002487(k)/A002487(k+1)) mod 2); S is summed to 4n+64
 *            binary places with fixed-point limbs
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Validated: partial, pass, 98 terms through n = 98 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    size_t K = (size_t)n * 4 + 64;
    size_t L = (K + 62) / 32 + 2;
    uint64_t *st = (uint64_t *)malloc((K + 3) * sizeof(uint64_t));
    uint32_t *v = (uint32_t *)calloc(L, sizeof(uint32_t));
    if (st == NULL || v == NULL) exit(1);

    st[0] = 0;
    st[1] = 1;
    for (size_t i = 2; i <= K + 1; ++i)
    {
        if ((i & 1) == 0) st[i] = st[i >> 1];
        else st[i] = st[i >> 1] + st[(i >> 1) + 1];
    }

    for (size_t k = 0; k <= K; ++k)
    {
        uint64_t b = st[k + 1];
        uint64_t mod = 2 * b;
        uint64_t result = 1 % mod, base = 2 % mod;
        size_t e = k;
        while (e > 0)
        {
            if (e & 1) result = (result * base) % mod;
            base = (base * base) % mod;
            e >>= 1;
        }
        uint64_t m = (result * (st[k] % mod)) % mod;
        if (m < b)
        {
            size_t p = 31 + k;
            v[p / 32] |= (uint32_t)1 << (31 - (p % 32));
        }
    }

    unsigned digit = v[0];
    for (idx_t d = 1; d < n; ++d)
    {
        v[0] = 0;
        uint64_t carry = 0;
        for (size_t i = L; i-- > 0;)
        {
            uint64_t t = (uint64_t)v[i] * 10 + carry;
            v[i] = (uint32_t)t;
            carry = t >> 32;
        }
        digit = (unsigned)v[0];
    }

    free(st);
    free(v);
    return (seq_t)(digit % 10);
}
