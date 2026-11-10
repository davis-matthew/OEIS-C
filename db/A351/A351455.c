/*
 * A351455  a(n) = A064989(A001615(A003961(n))).
 * https://oeis.org/A351455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative, a(p^e) = A064989((q+1)*q^(e-1)) = p^(e-1) *
 *            A064989(q+1), q = nextPrime(p); A064989 is multiplicative with
 *            a(2^e)=1, a(r^e)=prevprime(r)^e for odd r
 * Range:     u8 n <= 511, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 107 terms through n = 9733 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A351455_isp(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A351455_nextp(uint64_t p)
{
    uint64_t x = p + 1;
    while (!A351455_isp(x)) x++;
    return x;
}

static uint64_t A351455_prevp(uint64_t r)
{
    uint64_t x = r - 1;
    while (!A351455_isp(x)) x--;
    return x;
}

static seq_t A351455_A064989(uint64_t m)
{
    seq_t res = 1;
    uint64_t r;
    for (r = 2; r * r <= m; r++) {
        if (m % r == 0) {
            uint64_t e = 0;
            while (m % r == 0) { m /= r; e++; }
            if (r != 2) {
                seq_t b = (seq_t)A351455_prevp(r);
                while (e--) res *= b;
            }
        }
    }
    if (m > 2) res *= (seq_t)A351455_prevp(m);
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    uint64_t m = (uint64_t)n;
    uint64_t p;
    if (m < 1) return 0;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            result *= A351455_A064989(A351455_nextp(p) + 1);
            while (--e) result *= (seq_t)p;
        }
    }
    if (m > 1)
        result *= A351455_A064989(A351455_nextp(m) + 1);
    return result;
}
