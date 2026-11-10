/*
 * A369032  LCM-transform of permutation A241909.
 * https://oeis.org/A369032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014963(A241909(n)); equivalently a(1)=1, and
 *            a(n)=prime(Omega(n)) if n = 2^(k-1)*p with p prime (k=Omega(n)),
 *            otherwise a(n)=1
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9964 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A369032_isprime(idx_t m)
{
    if (m < 2) return false;
    for (idx_t i = 2; i <= m / i; ++i)
        if (m % i == 0) return false;
    return true;
}

static idx_t A369032_nth_prime(idx_t k)
{
    idx_t cnt = 0, c = 1;
    while (cnt < k) {
        ++c;
        if (A369032_isprime(c)) ++cnt;
    }
    return c;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    idx_t c = 0;
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
        ++c;
    }
    if (m == 1)
        return (seq_t)A369032_nth_prime(c);
    if (A369032_isprime(m))
        return (seq_t)A369032_nth_prime(c + 1);
    return (seq_t)1;
}
