/*
 * A350156  Inverse Moebius transform of A000056.
 * https://oeis.org/A350156
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e || n of * a(p^e) = p^(3e) -
 *            (p-1)*(p^(3e)-1)/(p^3-1) * * Algorithm: factor n by trial
 *            division, compute each prime‑power * contribution using the
 *            above formula and multiply them together. * Works for any n >= 1
 *            and for all supported seq_t sizes
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 842, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t r = 1;
    while (exp--) r *= base;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 1) return 1;                /* a(1) = 1 */

    seq_t result = 1;
    idx_t temp = n;

    /* 2 is the only even prime */
    if ((temp & 1) == 0) {
        uint64_t p = 2;
        idx_t e = 0;
        while ((temp % p) == 0) { temp /= p; ++e; }
        /* compute a(2^e) */
        seq_t p_seq = (seq_t) p;
        idx_t exp = 3 * e;
        seq_t powp = pow_seq(p_seq, exp);             /* p^(3e)      */
        seq_t denom = pow_seq(p_seq, 3) - 1;           /* p^3 - 1     */
        seq_t sub = (seq_t)(p - 1) * (powp - 1);       /* (p-1)(p^(3e)-1) */
        seq_t part = sub / denom;
        seq_t contrib = powp - part;
        result *= contrib;
    }

    /* odd factors */
    for (uint64_t f = 3; f <= temp / f; f += 2) {
        if (temp % f != 0) continue;
        uint64_t p = f;
        idx_t e = 0;
        while (temp % p == 0) { temp /= p; ++e; }
        seq_t p_seq = (seq_t) p;
        idx_t exp = 3 * e;
        seq_t powp = pow_seq(p_seq, exp);             /* p^(3e)      */
        seq_t denom = pow_seq(p_seq, 3) - 1;           /* p^3 - 1     */
        seq_t sub = (seq_t)(p - 1) * (powp - 1);       /* (p-1)(p^(3e)-1) */
        seq_t part = sub / denom;
        seq_t contrib = powp - part;
        result *= contrib;
    }

    /* if a prime factor remains (temp > 1) */
    if (temp > 1) {
        uint64_t p = temp;
        idx_t e = 1;
        seq_t p_seq = (seq_t) p;
        idx_t exp = 3 * e;
        seq_t powp = pow_seq(p_seq, exp);             /* p^(3)      */
        seq_t denom = pow_seq(p_seq, 3) - 1;           /* p^3 - 1     */
        seq_t sub = (seq_t)(p - 1) * (powp - 1);       /* (p-1)(p^3-1) */
        seq_t part = sub / denom;
        seq_t contrib = powp - part;
        result *= contrib;
    }

    return result;
}
