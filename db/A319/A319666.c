/*
 * A319666  Irregular triangle read by rows: T(n,k) is the discrete logarithm
 *          log_(-3)(4*k + 1) in the cyclic group 1+4Z/2^nZ, n >= 2, 0 <= k <=
 *          2^(n-2) - 1.
 * https://oeis.org/A319666
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where * r = ceil(log2((n-1)+1)) + 1 * k = (n-1) -
 *            2^(r-2) * T(r,k) is the smallest e such that (-3)^e ≡ 4*k+1 (mod
 *            2^r)
 * Range:     u8 n <= 513, u16 n <= 9780, u32 n <= 9780, u64 n <= 9780,
 *            u128 n <= 9780, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation works for any seq_t/idx_t provided by the build,
 * base on the fact that the required row numbers never exceed 63
 * for n ≤ 10 000.  The ternary search is taken directly from the
 * definition of the discrete logarithm in the 1+4Z/2^nZ group. */

#include <stdint.h>
#include <stdbool.h>

/* Helper: floor of log₂ for a positive 64‑bit integer. */
static inline uint64_t floor_log2_u64(uint64_t x)
{
    uint64_t log = 0;
    while (x >>= 1) ++log;
    return log;
}

/* Helper: ceil of log₂ for a positive 64‑bit integer. */
static inline uint64_t ceil_log2_u64(uint64_t x)
{
    if (x <= 1) return 0;
    uint64_t fl = floor_log2_u64(x);
    return (x & (x - 1)) == 0 ? fl : fl + 1;
}

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;

    /* Position in the linearised triangle (1‑based). */
    uint64_t p = (uint64_t)(n - 1);

    /* Row number r.  r = ceil(log₂(p+1)) + 1. */
    uint64_t r = ceil_log2_u64(p + 1) + 1;

    /* Index inside the row k.  k = p - 2^(r-2). */
    uint64_t k = p - ((uint64_t)1 << (r - 2));

    /* Modulus 2^r and target value 4*k+1 (mod 2^r). */
    uint64_t mod   = ((uint64_t)1 << r);
    uint64_t mask  = mod - 1;
    uint64_t target = (uint64_t)(4 * k + 1) & mask;

    /* Multiplicative factor (-3) modulo 2^r = mod - 3. */
    uint64_t factor = mod - 3;

    /* Brute‑force search for the discrete logarithm. */
    uint64_t cur   = 1;
    uint64_t limit = ((uint64_t)1 << (r - 2));
    for (uint64_t e = 0; e < limit; ++e) {
        if (cur == target) return (seq_t)e;
        cur = (cur * factor) & mask;
    }

    /* The definition guarantees a solution; this is unreachable. */
    return (seq_t)0;
}
