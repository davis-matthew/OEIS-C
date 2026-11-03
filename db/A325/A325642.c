/*
 * A325642  a(1) = 1; for n > 1, a(n) = k for the least divisor d > 1 of n
 *          such that A048720(d,k) = n for some k.
 * https://oeis.org/A325642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 511, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 111 terms through n = 9946 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A325642: a(1)=1.  For n>1, a(n) is the quotient k such that
 *   the binary polynomial of the least divisor d>1 of n (d | n)
 *   divides the binary polynomial of n over GF(2).
 *   The quotient polynomial evaluated as a binary integer is k.
 *   If no proper divisor satisfies, the divisor is n itself and k=1.
 *
 * This implementation works for any seq_t (builtin or big integer)
 * and any idx_t (here uint64_t).  It avoids dynamic allocation of
 * seq_t objects, so it compiles in both C11 and C++17.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return degree of a 64‑bit integer (position of highest set bit)
 * or 0 for value 0 (not used here). */
static int degree_u64(uint64_t v)
{
    int d = 0;
    while (v >>= 1) ++d;
    return d;
}

/* Divide the binary polynomial of n by the binary polynomial of d
 * over GF(2).  If divisible, return the quotient polynomial
 * represented as a binary integer; otherwise return 0. */
static seq_t poly_divide_u64(uint64_t n, uint64_t d)
{
    if (d == 0) return (seq_t)0;            /* undefined, return 0 */
    int deg_n = degree_u64(n);
    int deg_d = degree_u64(d);
    uint64_t rem = n;
    uint64_t quo = 0;
    while (rem != 0 && deg_n >= deg_d) {
        int shift = deg_n - deg_d;
        rem ^= d << shift;
        quo |= 1ULL << shift;
        if (rem == 0) break;
        deg_n = degree_u64(rem);
    }
    return (rem == 0) ? (seq_t)quo : (seq_t)0;
}

/* Public function required by the checker. */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;          /* a(1)=1 */

    uint64_t un = (uint64_t)n;
    /* Scan divisors in increasing order. */
    for (uint64_t d = 2; d <= un; ++d) {
        if (un % d != 0) continue;       /* not a divisor */
        seq_t q = poly_divide_u64(un, d);
        if (q != 0) return q;            /* first divisor that works */
    }
    /* If none found, n divides itself: return 1. */
    return (seq_t)1;
}
