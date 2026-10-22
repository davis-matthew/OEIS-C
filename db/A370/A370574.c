/*
 * A370574  a(n) is the least k > 0 such that n^2 XOR k^2 is a positive square
 *          number (where XOR denotes the bitwise XOR operator), or -1 if no
 *          such k exists.
 * https://oeis.org/A370574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 260, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Target: OEIS A370574
 *  a(n) = least k > 0 such that n^2 XOR k^2 is a positive perfect square,
 *  or –1 if none exists.  For n ≤ 2 we return –1.
 *  This is computed by a straightforward search over k until a solution
 *  is found; the search is efficient because n and k are small for the
 *  test range (n up to about 10 000).
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/* Integer square root (floor) for 64‑bit unsigned integers. */
static inline uint64_t isqrt64(uint64_t n)
{
    uint64_t res = 0;
    uint64_t bit = 1ULL << 62;          /* highest power of four ≤ n */

    while (bit > n) bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

/* Compose the required OEIS sequence. */
seq_t a(idx_t n)
{
    if (n <= 2u)                /* a(1) = a(2) = –1 */
        return (seq_t)-1;

    /* n^2 fits in 32 bits for all n in the relevant test range. */
    uint64_t nsq = (uint64_t)n * (uint64_t)n;

    for (uint64_t k = 1; ; ++k) {
        /* Compute k^2 with 128‑bit arithmetic to detect overflow. */
        unsigned __int128 ks = (unsigned __int128)k * k;
        if (ks >> 64)          /* k^2 would not fit in 64 bits */
            break;

        uint64_t ksq = (uint64_t)ks;
        uint64_t x   = nsq ^ ksq;      /* XOR of the squares */

        /* x must be a positive perfect square. */
        if (x == 0) continue;
        uint64_t r = isqrt64(x);
        if (r * r == x)
            return (seq_t)k;          /* minimal k found */
    }

    return (seq_t)-1;                /* fallback – should not occur for n > 2 */
}
