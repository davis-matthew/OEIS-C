/*
 * A270431  Numbers n such that A048673(n) and A064216(n) are of opposite
 *          parity.
 * https://oeis.org/A270431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th m >= 1 such that A048673(m) and A064216(m) have
 *            opposite parity (search; parities computed via A003961(m) mod 4
 *            and 3 | 2m-1)
 * Range:     u8 n <= 130, u16 n <= 17000, u32 n <= 17000, u64 n <= 17000,
 *            u128 n <= 17000, big any n
 * Validated: standard, pass, 90 terms through n = 9179 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static bool A270431_isp(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d <= x / d; d += 2)
        if (x % d == 0) return false;
    return true;
}

static uint64_t A270431_nextp(uint64_t p)
{
    uint64_t q = p + 1;
    while (!A270431_isp(q)) ++q;
    return q;
}

/* parity of A048673(m): (A003961(m)+1)/2 odd iff A003961(m) % 4 == 1 */
static int A270431_par48673(uint64_t m)
{
    uint64_t prod = 1; /* mod 4 */
    uint64_t t = m;
    for (uint64_t p = 2; p <= t / p; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            uint64_t np = A270431_nextp(p) % 4;
            while (t % p == 0) {
                t /= p;
                prod = (prod * np) % 4;
            }
        }
    }
    if (t > 1) prod = (prod * (A270431_nextp(t) % 4)) % 4;
    return (int)(((prod + 1) / 2) & 1);
}

/* parity of A064216(m): even iff 3 divides 2m-1 */
static int A270431_par64216(uint64_t m)
{
    uint64_t t = 2 * m - 1;
    return (t % 3 == 0) ? 0 : 1;
}

seq_t a(idx_t n)
{
    static uint64_t *terms = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t scanned = 0;

    while (cnt < (size_t)n) {
        ++scanned;
        if (A270431_par48673(scanned) != A270431_par64216(scanned)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
            }
            terms[cnt++] = scanned;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
