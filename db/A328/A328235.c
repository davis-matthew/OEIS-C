/*
 * A328235  The least k > 0 such that the arithmetic derivative of n+k is a
 *          multiple of the arithmetic derivative of n.
 * https://oeis.org/A328235
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   least k > 0 such that the arithmetic derivative of n+k is a
 *            multiple of the arithmetic derivative of n. The arithmetic
 *            derivative of an integer m is defined by D(m) = Σ e_i * m / p_i
 *            where m = Π p_i ^ e_i
 * Range:     u8 n <= 39, u16 n <= 7013, u32 n <= 7509, u64 n <= 7509,
 *            u128 n <= 7509, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

/* Compute the arithmetic derivative of a 64‑bit integer n.
   The result is returned as a seq_t; for n ≤ 10000 it fits in 64 bits. */
static seq_t derivative_uint64(uint64_t n)
{
    if (n < 2)
        return (seq_t)0;          /* D(0) = D(1) = 0 by convention */

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(n, primes, exps, &cnt);

    seq_t sum = (seq_t)0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        sum += ((seq_t)(n / p)) * (seq_t)e;   /* e * (n/p) */
    }
    return sum;
}

/* Main sequence function.  idx_t is an unsigned 64‑bit integer type. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* values below the offset are undefined */

    seq_t dn = derivative_uint64((uint64_t)n);
    if (dn == 0)                 /* should not happen for n ≥ 2 */
        return (seq_t)0;

    uint64_t k = 1;
    for (;;) {
        uint64_t m = n + k;                       /* n and k are 64‑bit */
        seq_t dm = derivative_uint64(m);
        if (dm % dn == 0)
            return (seq_t)k;                     /* exact return value */
        ++k;
    }
}
