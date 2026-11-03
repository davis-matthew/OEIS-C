/*
 * A257303  Number of 5th power nonresidues modulo n.
 * https://oeis.org/A257303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 105 terms through n = 9920 (2026-11-01)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Number of 5th power nonresidues modulo n.
 * For each prime power p^e dividing n the number of 5th‑power residues is
 *   cnt(p^e) = 1 + Σ_{t=0}^{⌊(e-1)/5⌋} φ(p^{e-5t}) / gcd(5, φ(p^{e-5t}))
 *   (the “1” counts the zero residue).
 * The total number of residues is the product of cnt(p^e) over all prime powers.
 * a(n) is n minus that product.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static seq_t count_fifth_power_residues_prime_power(uint64_t p, idx_t e)
{
    /* count of residues modulo p^e that are 5th powers */
    seq_t rescnt = 1; /* for 0 */

    idx_t max_t = (e - 1) / 5;
    for (idx_t t = 0; t <= max_t; ++t) {
        idx_t e_t = e - 5 * t;                  /* exponent of the unit part */
        if (e_t == 0) continue;                 /* should not happen */
        /* φ(p^{e_t}) = p^{e_t-1} * (p-1) */
        seq_t pow = 1;
        for (idx_t i = 0; i < e_t - 1; ++i)
            pow *= (seq_t)p;                    /* compute p^{e_t-1} */
        seq_t phi = pow * (p - 1);              /* φ value */

        /* gcd(5, φ) : φ % 5 tells if 5 divides φ */
        seq_t g = (phi % 5 == 0) ? 5 : 1;
        rescnt += phi / g;
    }
    return rescnt;
}

seq_t a(idx_t n)
{
    if (n == 1) return 0;  /* n = 1 yields only the residue 0 */

    uint64_t N = (uint64_t)n;

    /* factor n */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(N, primes, exps, &cnt);

    seq_t residues = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        idx_t    e = (idx_t)exps[i];
        residues *= count_fifth_power_residues_prime_power(p, e);
    }

    seq_t result = (seq_t)n - residues;
    return result;
}
