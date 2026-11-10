/*
 * A354204  a(n) = phi(A354202(n)), where A354202 is fully multiplicative with
 *          a(p) = A354200(A000720(p)).
 * https://oeis.org/A354204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   multiplicative, a(p^e) = (q-1)*q^(e-1), q = A354200(A000720(p))
 *            = next prime > p with q == p (mod 4) (q = 5 for p = 2)
 * Range:     u8 n <= 15, u16 n <= 191, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9778 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A354204_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A354204_map(uint64_t p)
{
    if (p == 2) return 5;
    uint64_t q = p + 4;
    while (!A354204_isprime(q)) q += 4;
    return q;
}

static void A354204_term(seq_t *res, uint64_t p, idx_t e)
{
    uint64_t q = A354204_map(p);
    *res *= (seq_t)(q - 1);
    while (--e > 0) *res *= (seq_t)q;
}

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t m = n;
    if (m == 0) return 0;
    for (idx_t p = 2; p <= m / p; p++) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            A354204_term(&result, (uint64_t)p, e);
        }
    }
    if (m > 1) A354204_term(&result, (uint64_t)m, 1);
    return result;
}
