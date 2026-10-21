/*
 * A278260  Least number with the same prime signature as {the n-th
 *          quarter-square}+1.
 * https://oeis.org/A278260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A033638(n))
 * Range:     u8 n <= 15, u16 n <= 267, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, get_prime, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A033638(n) = floor(n^2 / 4) */
#include <stddef.h>
#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* quarter‑square + 1 */
    seq_t nsq = (seq_t)n * (seq_t)n;
    seq_t q   = nsq / 4 + 1;

    /* factor q – q ≤ 25 000 001 for n ≤ 10 000 */
    uint64_t primes[32], exps[32];
    size_t cnt = 0;
    factor_u64((uint64_t)q, primes, exps, &cnt);

    if (cnt == 0) {
        /* q = 1 */
        return (seq_t)1;
    }

    /* sort exponents in non‑increasing order */
    for (size_t i = 0; i < cnt; ++i) {
        for (size_t j = i + 1; j < cnt; ++j) {
            if (exps[i] < exps[j]) {
                uint64_t t = exps[i];
                exps[i]    = exps[j];
                exps[j]    = t;
            }
        }
    }

    /* minimal number with the same prime signature */
    seq_t res = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t prime = (seq_t)get_prime((idx_t)(i + 1));
        seq_t pows  = pow_uint((uint64_t)prime, exps[i]);
        res *= pows;
    }

    return res;
}
