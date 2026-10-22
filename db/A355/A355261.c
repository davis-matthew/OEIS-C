/*
 * A355261  a(n) = largest-nth-power(n, 2) * radical(n) = A000188(n) *
 *          A007947(n), where largest-nth-power(n, e) is the largest positive
 *          integer b such that b^e divides n.
 * https://oeis.org/A355261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = largest-nth-power(n,2) * radical(n) Multiplicative
 *            definition: a(p^e) = p^(1+floor(e/2)) (p prime)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
        res = seq_mul(res, base);
    return res;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1) = 1, handled naturally by the loop below */
    if (n < 2)
        return 1;

    seq_t result = 1;

    /* Factor n – the table size 64 is ample for 64‑bit integers */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t   cnt = 0;

    factor_u64(n, primes, exps, &cnt);

    for (size_t i = 0; i < cnt; ++i) {
        idx_t  power = 1 + (exps[i] / 2);          /* 1 + floor(e/2)   */
        seq_t  term  = pow_seq((seq_t)primes[i], power);
        result = seq_mul(result, term);
    }

    return result;
}
