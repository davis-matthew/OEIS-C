/*
 * A294890  Number of divisors of n that are primitively abundant (A091191).
 * https://oeis.org/A294890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d|n} 1 if d is primitively abundant (A091191); 0
 *            otherwise
 * Range:     u8 n <= 9258, u16 n <= 9258, u32 n <= 9258, u64 n <= 9258,
 *            u128 n <= 9258, big any n
 * Helpers:   factor_u64, generate_divisors_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helpers */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/generate_divisors_u64.h"

/* Estimate the maximum number of divisors for n up to 10⁴ ( < 256 ) */
#define MAX_DIVISORS 1024

/* Test whether the number m is abundant.
   Returns true iff the sum of proper divisors of m exceeds m. */
static bool is_abundant(idx_t m)
{
    if (m <= 1) return false;

    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_u64((uint64_t)m, pr, ex, &cnt);

    uint64_t sigma = 1;       /* σ(m) */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        unsigned int e = (unsigned int)ex[i];

        uint64_t term = 1;    /* 1 + p + p² + ... + pᵉ */
        uint64_t pw = 1;
        for (unsigned int k = 1; k <= e; ++k) {
            pw *= p;
            term += pw;
        }
        sigma *= term;
    }

    uint64_t proper = sigma - (uint64_t)m;
    return proper > (uint64_t)m;
}

/* Test whether the number m is primitively abundant.
   It must be abundant itself and none of its proper divisors is abundant. */
static bool is_primitive_abundant(idx_t m)
{
    if (m <= 1) return false;
    if (!is_abundant(m)) return false;

    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_u64((uint64_t)m, pr, ex, &cnt);

    idx_t divs[MAX_DIVISORS];
    size_t divsz;
    generate_divisors_u64(pr, ex, cnt, divs, &divsz);

    for (size_t i = 0; i < divsz; ++i) {
        idx_t d = divs[i];
        if (d == m) continue;      /* skip m itself */
        if (is_abundant(d)) return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    uint64_t pr[32], ex[32];
    size_t cnt;
    factor_u64((uint64_t)n, pr, ex, &cnt);

    idx_t divs[MAX_DIVISORS];
    size_t divsz;
    generate_divisors_u64(pr, ex, cnt, divs, &divsz);

    seq_t ans = 0;
    for (size_t i = 0; i < divsz; ++i) {
        if (is_primitive_abundant(divs[i])) ans++;
    }
    return ans;
}
