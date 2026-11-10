/*
 * A339970  a(n) = A329697(A019565(2n)).
 * https://oeis.org/A339970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k in bits of n} A329697(prime(k+2)), i.e.
 *            A329697(A019565(2n)) with A329697(m) = 0 if m is a power of 2,
 *            else 1 + A329697(m - m/lpf(m))
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 122 terms through n = 9779 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A339970_lpf(uint64_t m)
{
    uint64_t largest = 1;
    while ((m & 1ULL) == 0) {
        largest = 2;
        m >>= 1;
    }
    for (uint64_t f = 3; f <= m / f; f += 2) {
        if (m % f == 0) {
            largest = f;
            while (m % f == 0)
                m /= f;
        }
    }
    if (m > 1)
        largest = m;
    return largest;
}

static unsigned A339970_A329697(uint64_t m)
{
    unsigned c = 0;
    while (m > 1 && (m & (m - 1)) != 0) {
        uint64_t p = A339970_lpf(m);
        m -= m / p;
        c++;
    }
    return c;
}

static bool A339970_isprime(uint64_t x)
{
    if (x < 2) return false;
    for (uint64_t d = 2; d * d <= x; d++)
        if (x % d == 0) return false;
    return true;
}

/* idx-th prime, 1-indexed (prime(1) = 2) */
static uint64_t A339970_prime(unsigned idx)
{
    uint64_t x = 1;
    unsigned c = 0;
    while (c < idx) {
        x++;
        if (A339970_isprime(x)) c++;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t total = 0;
    unsigned k = 0;
    while (n != 0) {
        if (n & 1ULL) {
            total += (seq_t)A339970_A329697(A339970_prime(k + 2));
        }
        n >>= 1;
        k++;
    }
    return total;
}
