/*
 * A319665  Irregular triangle read by rows: T(n,k) is the discrete logarithm
 *          log_5(4*k + 1) in the cyclic group 1+4Z/2^nZ, n >= 2, 0 <= k <=
 *          2^(n-2) - 1.
 * https://oeis.org/A319665
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = log_5(4*k+1) in the group Z/(2^n) where r =
 *            floor(log2(n-1)) + 2 is the row containing the term, and k =
 *            (n-2) - (2^(r-2) - 1) is the column within that row
 * Range:     u8 n <= 514, u16 n <= 8034, u32 n <= 8034, u64 n <= 8034,
 *            u128 n <= 8034, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper: floor of log2 for a 64‑bit value. */
static unsigned floor_log2_u64(uint64_t x)
{
    unsigned r = 0;
    while (x >>= 1) ++r;
    return r;
}

 seq_t a(idx_t n)
{
    /* The OEIS offset is 2 → a(2) is the first term. */
    if (n < 2) return (seq_t)0;

    /* Determine the row number r and the column index k. */
    uint64_t N    = (uint64_t)n;              /* avoid signed overflow */
    unsigned r = floor_log2_u64(N - 1) + 2;    /* r >= 2 and fits in a 64‑bit type */
    uint64_t prefix = ((uint64_t)1 << (r - 2)) - 1;  /* terms before row r */
    uint64_t k = (N - 2) - prefix;                     /* 0 <= k < 2^(r-2) */

    /* Target value: 4*k + 1 (always odd and ≡ 1 mod 4). */
    uint64_t target = 4 * k + 1;

    /* Special case: exponent 0 corresponds to the element 1. */
    if (target == 1) return (seq_t)0;

    /* Compute the discrete logarithm: find e such that 5^e ≡ target (mod 2^r).
       Order of 5 modulo 2^r is 2^(r-2). */
    uint64_t max_exp = ((uint64_t)1 << (r - 2));          /* upper bound exclusive */
    uint64_t modulus;
    uint64_t mask;
    if (r == 64) {                                       /* modulo 2^64 is natural overflow */
        modulus = 0;
        mask    = ~0ULL;
    } else {
        modulus = ((uint64_t)1 << r);
        mask    = modulus - 1;
    }

    uint64_t power = 1;          /* 5^0 mod 2^r */
    for (uint64_t e = 1; e < max_exp; ++e) {
        power = (power * 5UL) & mask;   /* next power of 5 modulo 2^r */
        if (power == target) {
            return (seq_t)e;            /* found the exponent */
        }
    }

    /* The condition guarantees a solution in [0,2^(r-2)-1], so reaching here is impossible. */
    return (seq_t)0;
}
