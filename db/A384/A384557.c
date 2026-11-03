/*
 * A384557  The number of exponential unitary (or e-unitary) divisors of n
 *          that are exponentially odd numbers (A268335).
 * https://oeis.org/A384557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9991 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = product_{p^e || n} 2^{omega_odd(e)},
 * where omega_odd(e) is the number of distinct odd prime factors of e.
 *
 * Implementation: factor n, for each prime power p^e compute t = |{q | q|e, q odd, q prime}|
 * and multiply the result by (1 << t).  Since for n ≤ 10000 each exponent e ≤ 13,
 * t ≤ 1 and the product fits easily in seq_t.  The algorithm is O(sqrt(n))
 * per call and works for any 64‑bit n.
 */

#include <stdint.h>

static seq_t exponent_factor(uint64_t e)
{
    /* Count distinct odd prime factors of e. */
    unsigned t = 0;

    /* Remove all factors of 2. */
    while ((e & 1ULL) == 0 && e > 1)
        e >>= 1;

    /* Factor remaining odd part. */
    uint64_t d;
    for (d = 3; d * d <= e; d += 2) {
        if (e % d == 0) {
            t++;
            while (e % d == 0)
                e /= d;
        }
    }
    if (e > 1)
        t++;

    return (seq_t)((seq_t)1 << t);   /* 1 << t is safe because t is tiny. */
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                      /* a(1) = 1 */

    uint64_t m = (uint64_t)n;
    seq_t result = 1;

    /* Handle factor 2 separately. */
    unsigned exp = 0;
    while ((m & 1ULL) == 0 && m > 0) {
        exp++;
        m >>= 1;
    }
    if (exp > 0)
        result *= exponent_factor((uint64_t)exp);

    /* Handle odd prime factors. */
    for (uint64_t p = 3; p * p <= m; p += 2) {
        if (m % p == 0) {
            exp = 0;
            while (m % p == 0) {
                exp++;
                m /= p;
            }
            result *= exponent_factor((uint64_t)exp);
        }
    }

    /* If a prime factor remains (>1). */
    if (m > 1)
        result *= exponent_factor(1ULL);   /* exponent 1 contributes factor 1 */

    return result;
}
