/*
 * A364107  Expansion of Sum_{k>0} k * x^(4*k) / (1 - x^(5*k-1)).
 * https://oeis.org/A364107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/5) * sum_{d | (5*n-4), d≡4 (mod 5)} (d+1)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(x)) */
static inline uint64_t isqrt64(uint64_t x) {
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x)
        ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Compute M = 5*n - 4, using 64‑bit arithmetic to avoid overflow
       for n within the intended range */
    uint64_t M = 5ull * n - 4ull;
    seq_t sum = 0;

    /* Enumerate divisors of M up to its square root */
    uint64_t limit = isqrt64(M);
    for (uint64_t d = 1; d <= limit; ++d) {
        if (M % d == 0) {
            uint64_t d1 = d;
            uint64_t d2 = M / d;
            if (d1 % 5 == 4ULL)
                sum += ((seq_t)d1 + 1);
            if (d2 != d1 && d2 % 5 == 4ULL)
                sum += ((seq_t)d2 + 1);
        }
    }

    return sum / 5;
}
