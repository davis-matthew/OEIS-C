/*
 * A342435  a(n) = Sum_{k=1..n} gcd(k,n)^(gcd(k,n) - 2).
 * https://oeis.org/A342435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{d|n} φ(n/d) · d^(d-2) (OEIS A342435)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 18,
 *            big any n
 * Helpers:   calc_phi, factor_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Utility functions from the utils library */
#include "../utils/factor_u64.h"   /* void factor_u64(uint64_t , uint64_t *, uint64_t *, size_t *); */
#include "../utils/calc_phi.h"    /* seq_t calc_phi(seq_t ); */
#include "../utils/seq_pow.h"    /* seq_t seq_pow(seq_t , idx_t ); */

/* Enumerate all divisors of a number using its prime factorisation. */
static void gen_divisors(size_t idx,
                         uint64_t cur,
                         const uint64_t *pr,
                         const uint64_t *ex,
                         size_t pcnt,
                         uint64_t *divs,
                         size_t *cnt)
{
    if (idx == pcnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t val = cur;
    for (uint64_t i = 0; i <= ex[idx]; ++i) {
        gen_divisors(idx + 1, val, pr, ex, pcnt, divs, cnt);
        val *= pr[idx];
    }
}

/* Main sequence function (external linkage). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* safety for off‑by‑one use */

    /* 1. Factorise n. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    /* 2. Generate all divisors of n. */
    uint64_t divisors[128];
    size_t dcnt = 0;
    gen_divisors(0, 1, primes, exps, pcnt, divisors, &dcnt);

    /* 3. Accumulate the sum. */
    seq_t sum = 0;
    for (size_t i = 0; i < dcnt; ++i) {
        uint64_t d = divisors[i];
        seq_t phi = calc_phi((seq_t)(n / d));

        seq_t pow_term;
        if (d <= 2) {
            /* d = 1 → exponent = –1, treat 1^(–1) = 1
               d = 2 → exponent =  0, so 2^0 = 1 */
            pow_term = (seq_t)1;
        } else {
            pow_term = seq_pow((seq_t)d, (idx_t)(d - 2));
        }

        sum += phi * pow_term;   /* allowed operators: * and + */
    }

    return sum;
}
