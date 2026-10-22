/*
 * A345052  a(n) = A003557(n) * A048250(n) * A173557(n).
 * https://oeis.org/A345052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Multiplicative function: a(p^e) = (p^2 - 1) * p^(e-1) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)1;                 /* offset starts at 1 */

    /* Work on a 64‑bit copy of n for factorization. */
    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* ----- factor 2 explicitly ----- */
    int e = 0;
    while ((m & 1ULL) == 0) {
        m >>= 1;
        ++e;
    }
    if (e > 0) {
        seq_t base = (seq_t)2;
        seq_t term = base * base - (seq_t)1;      /* 2^2-1 = 3 */
        seq_t pow_part = 1;
        for (int i = 1; i < e; ++i)               /* p^(e-1)  */
            pow_part *= base;
        result *= term * pow_part;
    }

    /* ----- factor odd primes ----- */
    uint64_t p = 3;
    while (p <= m / p) {                         /* p*p <= m without overflow */
        if (m % p == 0) {
            e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t base = (seq_t)p;
            seq_t term = base * base - (seq_t)1;  /* p^2-1 */
            seq_t pow_part = 1;
            for (int i = 1; i < e; ++i)
                pow_part *= base;
            result *= term * pow_part;
        }
        p += 2;
    }

    /* ----- remaining prime factor (if any) ----- */
    if (m > 1) {
        seq_t base = (seq_t)m;                    /* exponent 1 */
        seq_t term = base * base - (seq_t)1;      /* m^2-1 */
        result *= term;                           /* pow_part is 1 */
    }

    return result;
}
