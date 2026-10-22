/*
 * A392169  The Dedekind psi function value of the smallest cube divisible by
 *          n.
 * https://oeis.org/A392169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = Dedekind psi of the smallest perfect cube divisible by n.
 *            Computed multiplicatively: if n = ∏ p^e, let shift = (3 - e % 3)
 *            % 3, then a(n) = ∏ (p + 1) * p^(e + shift - 1)
 * Range:     u8 n <= 5, u16 n <= 37, u32 n <= 1229, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Fast power for seq_t. */
static seq_t pow_seq(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    uint64_t orig = n;           /* keep original for loop bound */
    uint64_t nn   = n;           /* will be divided by prime factors */
    seq_t result = 1;            /* accumulating product */

    for (uint64_t p = 2; p <= orig / p; ++p) {
        if (nn % p != 0) continue;

        /* Count exponent e of prime p in n. */
        uint64_t e = 0;
        while (nn % p == 0) {
            nn /= p;
            ++e;
        }

        /* Determine shift to next multiple of 3. */
        uint64_t r     = e % 3;
        uint64_t shift = (r == 0) ? 0          : (r == 1) ? 2 : 1;
        uint64_t exp   = e + shift - 1;        /* exponent in ψ(m) */

        seq_t base  = (seq_t)p;
        seq_t term  = pow_seq(base, exp) * (base + 1);
        result     *= term;
    }

    /* Remaining prime factor, if any, has exponent 1. */
    if (nn > 1) {
        uint64_t e = 1;
        uint64_t r     = e % 3;                /* r = 1 */
        uint64_t shift = (r == 0) ? 0          : (r == 1) ? 2 : 1;   /* shift = 2 */
        uint64_t exp   = e + shift - 1;        /* exp = 2 */

        seq_t base  = (seq_t)nn;
        seq_t term  = pow_seq(base, exp) * (base + 1);
        result     *= term;
    }

    return result;
}
