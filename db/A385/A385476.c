/*
 * A385476  Values of u in the quartets (1, u, v, w) of type 3; i.e., values
 *          of u for solutions to (1 - u) = v(v - w), in distinct positive
 *          integers, with v > 1, sorted by nondecreasing values of u; see
 *          Comments.
 * https://oeis.org/A385476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the fact that a(n) is the nth term of the multiset
 * { u = v*d + 1 | v > 1, d > 1 } sorted by non‑decreasing u.
 * For each product N = v*d, the multiplicity contributed is
 * τ(N) – 2, where τ is the number of positive divisors of N.
 * The function accumulates these multiplicities until the
 * desired index n is reached, returning u = N+1. */

#include <stdint.h>
#include <stdbool.h>

/* Return the number of positive divisors of n. */
static uint64_t divisor_count(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t tmp  = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint32_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (tmp > 1) {
        tau *= 2;        /* remaining prime factor */
    }
    return tau;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* offset starts at 1 */

    idx_t count = 0;
    for (uint64_t N = 4; ; ++N) {       /* N = u – 1, N ≥ 4 */
        uint64_t tau = divisor_count(N);
        if (tau <= 2) continue;         /* product has no valid factors */
        uint64_t mult = tau - 2;        /* multiplicity of this u */

        if (count + mult >= n) {
            return (seq_t)(N + 1);      /* u = N + 1 */
        }
        count += mult;
    }
}
