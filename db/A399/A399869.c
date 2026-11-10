/*
 * A399869  Number of steps required for n to reach 1 under repeated
 *          application of A048675, which encodes the exponents in the prime
 *          factorization of n as a binary integer; or -1 if 1 is never
 *          reached.
 * https://oeis.org/A399869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(1) = 0; a(n) = 1 + a(A048675(n)) for n >= 2 (iteration of
 *            A048675 until 1 is reached; -1 if never)
 * Range:     u8 n <= 37, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Validated: partial, pass, 37 terms through n = 37 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* A048675 on small values; returns false if the result is out of range */
static bool A048675_step(uint64_t m, uint64_t *out)
{
    unsigned __int128 sum = 0;
    uint64_t p;
    for (p = 2; p * p <= m || m > 1; p++) {
        if (p > 2000000ULL) return false;
        if (m % p) {
            if (p * p > m) {
                /* m is prime (>1) */
                p = m;
            } else continue;
        }
        uint64_t e = 0;
        while (m % p == 0) { m /= p; e++; }
        /* prime index of p */
        uint64_t idx = 0;
        for (uint64_t q = 2; q <= p; q++) {
            bool pr = true;
            for (uint64_t d = 2; d * d <= q; d++)
                if (q % d == 0) { pr = false; break; }
            if (pr) idx++;
        }
        if (idx > 60) return false;
        sum += (unsigned __int128)e << (idx - 1);
        if (sum >> 62) return false;
        if (m == 1) break;
    }
    *out = (uint64_t)sum;
    return true;
}

seq_t a(idx_t n)
{
    uint64_t v = (uint64_t)n;
    uint64_t steps = 0;
    while (v != 1) {
        uint64_t nx;
        if (!A048675_step(v, &nx)) return (seq_t)(-1);
        v = nx;
        steps++;
        if (steps > 100000) return (seq_t)(-1);
    }
    return (seq_t)steps;
}
