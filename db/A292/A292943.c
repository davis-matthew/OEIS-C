/*
 * A292943  a(n) = A292944(A243071(n)); Base-2 expansion of a(n) encodes the
 *          steps where numbers of the form 6k+3 are encountered when map x ->
 *          A252463(x) is iterated down to 1, starting from x=n.
 * https://oeis.org/A292943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; a(n) = 2*a(A252463(n)) + [n == 3 (mod 6)] for n > 1
 * Range:     u8 n <= 28, u16 n <= 60, u32 n <= 138, u64 n <= 316,
 *            u128 n <= 732, big any n
 * Validated: standard, pass, 98 terms through n = 1937 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool a292943_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t a292943_prevprime(uint64_t p)
{
    uint64_t q = p - 1;
    while (!a292943_isprime(q)) --q;
    return q;
}

static uint64_t a292943_step(uint64_t n)
{
    if (n == 1) return 1;
    if ((n & 1) == 0) return n >> 1;
    uint64_t m = n, result = 1;
    for (uint64_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            uint64_t base = a292943_prevprime(p);
            while (m % p == 0) {
                m /= p;
                result *= base;
            }
        }
    }
    if (m > 1) result *= a292943_prevprime(m);
    return result;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    seq_t res = 0;
    seq_t pw = 1;
    while (x > 1) {
        if (x % 6 == 3) res += pw;
        pw += pw;
        x = a292943_step(x);
    }
    return res;
}
