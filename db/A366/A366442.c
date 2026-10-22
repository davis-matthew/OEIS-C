/*
 * A366442  The sum of divisors of the 5-rough numbers (A007310).
 * https://oeis.org/A366442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(A007310(n))
 * Range:     u8 n <= 5, u16 n <= 1106, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the sum of divisors of the n-th 5‑rough number (numbers coprime to 6). */
#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

/* Return σ(m) where m is a 64‑bit integer. */
static seq_t sigma_of_uint64(uint64_t m)
{
    if (m == 0) return 0;      /* defensive, m is never 0 when called */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    seq_t sigma = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];
        seq_t term = (pow_uint(p, e + 1) - (seq_t)1) / ((seq_t)p - (seq_t)1);
        sigma *= term;
    }
    return sigma;
}

/*
 * nth 5‑rough number (numbers coprime to 6):
 *   for n odd : 6 * ((n-1)/2) + 1
 *   for n even: 6 * ((n/2)-1) + 5
 */
seq_t a(idx_t n)
{
    uint64_t rough;
    if (n & 1ULL) {
        rough = 6ULL * ((n - 1ULL) / 2ULL) + 1ULL;
    } else {
        rough = 6ULL * ((n / 2ULL) - 1ULL) + 5ULL;
    }
    return sigma_of_uint64(rough);
}
