/*
 * A340901  Additive with a(p^e) = (-p)^e.
 * https://oeis.org/A340901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = \sum_{p^e || n} (-p)^e, a(1) = 0
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is additive: for coprime m,n, a(mn) = a(m)+a(n). */

#include <stdint.h>
#include "../utils/factor_u64.h"

/* Computes the sequence entry a(n) for the OEIS sequence A340901. */
seq_t a(idx_t n)
{
    /* a(1) = 0, handle early. */
    if (n == 1)
        return 0;

    /* Factor n into primes p_i and exponents e_i.  */
    /* 64 is plenty to hold all prime factors of a 64‑bit integer. */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 0;

    for (size_t i = 0; i < cnt; ++i) {
        seq_t p = (seq_t)pr[i];
        size_t e = (size_t)ex[i];

        /* Compute (-p)^e by repeated multiplication. */
        seq_t term = 1;
        for (size_t j = 0; j < e; ++j) {
            term *= -p;
        }

        result += term;
    }

    return result;
}
