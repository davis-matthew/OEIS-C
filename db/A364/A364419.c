/*
 * A364419  Number of divisors of n of the form 5*k+3 that are at most
 *          sqrt(n).
 * https://oeis.org/A364419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of n such that d ≡ 3 (mod 5) and d ≤
 *            √n
 * Range:     u8 n <= 63, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/floor_sqrt.h"

/* Recursively enumerate all divisors of n that do not exceed max,
   counting those that are congruent to 3 modulo 5. */
static void
rec_div(size_t pos, seq_t prod,
        const uint64_t *pr, const uint64_t *ex,
        size_t cntp, seq_t max, seq_t *out)
{
    if (pos == cntp) {            /* reached a combination of powers */
        if (prod <= max && (prod % (seq_t)5) == (seq_t)3)
            ++(*out);
        return;
    }
    seq_t p = (seq_t)pr[pos];
    seq_t cur = prod;
    for (size_t e = 0; e <= ex[pos]; ++e) {
        if (cur > max) break;
        rec_div(pos + 1, cur, pr, ex, cntp, max, out);
        /* prepare for next exponent: cur *= p, but avoid overflow */
        if (e == ex[pos]) break;          /* no further exponent needed */
        if (max / p < cur) break;         /* would exceed max */
        cur *= p;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    seq_t max = floor_sqrt((seq_t)n);

    /* factorize n */
    uint64_t primes[64];
    uint64_t exponents[64];
    size_t num = 0;
    factor_u64((uint64_t)n, primes, exponents, &num);

    seq_t count = 0;
    rec_div(0, (seq_t)1, primes, exponents, num, max, &count);
    return count;
}
