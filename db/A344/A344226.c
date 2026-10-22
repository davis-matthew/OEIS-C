/*
 * A344226  a(n) = Sum_{d|n} n^omega(d) / d.
 * https://oeis.org/A344226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} n^ω(d) / d * where ω(d) is the number of distinct
 *            prime factors of d
 * Range:     u8 n <= 17, u16 n <= 41, u32 n <= 1049, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Recursive enumeration of all divisors of n, given its prime factorisation. */
static void enumerate_divs(size_t idx, seq_t cur, unsigned w,
                           const uint64_t *pr, const uint64_t *ex,
                           size_t cnt, seq_t *sum, seq_t sn)
{
    if (idx == cnt) {
        seq_t contrib = pow_uint((uint64_t)sn, (idx_t)w) / cur;
        *sum += contrib;
        return;
    }
    /* Exponent 0: prime not present in this divisor. */
    enumerate_divs(idx + 1, cur, w, pr, ex, cnt, sum, sn);
    /* Exponents 1 .. ex[idx] */
    seq_t next = cur;
    for (unsigned e = 1; e <= ex[idx]; ++e) {
        next *= (seq_t)pr[idx];
        enumerate_divs(idx + 1, next, w + 1, pr, ex, cnt, sum, sn);
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Undefined for n = 0, but safe guard. */

    /* Prime factorisation of n. */
    uint64_t pr[16];
    uint64_t ex[16];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t sum = 0;
    seq_t sn = (seq_t)n;

    enumerate_divs(0, (seq_t)1, 0, pr, ex, cnt, &sum, sn);

    return sum;
}
