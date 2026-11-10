/*
 * A302850  Permutation of natural numbers: a(1) = 1; a(2) = 2; for n > 2,
 *          a(n) = 2+A064989(A003961(n)-4).
 * https://oeis.org/A302850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=2; for n>2, a(n) = 2 + A064989(A003961(n) - 4)
 * Range:     u8 n <= 47, u16 n <= 1535, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 102 terms through n = 7886 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A302850_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A302850_next(uint64_t p)
{
    uint64_t q = p + 1;
    while (!A302850_isp(q)) q++;
    return q;
}

static uint64_t A302850_prev(uint64_t p)
{
    uint64_t q = p - 1;
    while (!A302850_isp(q)) q--;
    return q;
}

/* A003961 computed in uint64 */
static uint64_t A302850_A003961(uint64_t m)
{
    uint64_t result = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = A302850_next(p);
            while (m % p == 0) {
                m /= p;
                result *= q;
            }
        }
    }
    if (m > 1) result *= A302850_next(m);
    return result;
}

/* A064989 over seq_t result */
static seq_t A302850_A064989(uint64_t m)
{
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) { m /= p; e++; }
            if (p != 2) {
                seq_t b = (seq_t)A302850_prev(p);
                while (e--) result *= b;
            }
        }
    }
    if (m > 2) result *= (seq_t)A302850_prev(m);
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 2;
    uint64_t t = A302850_A003961((uint64_t)n) - 4;
    return (seq_t)2 + A302850_A064989(t);
}
