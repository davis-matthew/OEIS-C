/*
 * A354202  Fully multiplicative with a(p^e) = A354200(A000720(p))^e.
 * https://oeis.org/A354202
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{p^e || n} q(p)^e, completely multiplicative,
 *            where q(2) = 5 and q(p) is the next larger prime congruent to p
 *            mod 4 for odd p (q(p) = A354200(A000720(p)))
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9773 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A354202_isprime(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A354202_map(uint64_t p)
{
    if (p == 2) return 5;
    uint64_t q = p + 4;
    while (!A354202_isprime(q)) q += 4;
    return q;
}

seq_t a(idx_t n)
{
    seq_t res = 1;
    uint64_t m = (uint64_t)n;
    if (m < 1) return 0;
    for (uint64_t d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            seq_t q = (seq_t)A354202_map(d);
            while (m % d == 0) {
                m /= d;
                res *= q;
            }
        }
    }
    if (m > 1) res *= (seq_t)A354202_map(m);
    return res;
}
