/*
 * A380950  a(n) = denominator(r(n)) where r(n) =
 *          (n/2)*(Pi/2)^cos(Pi*(n-1))*((n/2-1/2)!/(n/2)!)^2.
 * https://oeis.org/A380950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of r(n), where r(n) =
 *            (n/2)*(Pi/2)^cos(Pi*(n-1))*((n/2-1/2)!/(n/2)!)^2.
 *            Simplifications: - For even n = 2m: a(2m) = 2^{4m - (1 + v_2(m)
 *            + 2*popcount(m))} - For odd n = 2m+1: a(2m+1) = (2m+1) *
 *            (binom(2m, m) >> popcount(m))^2
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

/* Count the number of 1-bits in a 64‑bit integer. */
static inline unsigned int popcount_uint64(uint64_t x) {
#ifdef __GNUC__
    return __builtin_popcountll(x);
#else
    unsigned int c = 0;
    while (x) { x &= (x - 1); ++c; }
    return c;
#endif
}

/* Count trailing zeros (exponent of 2) in a 64‑bit integer.  The argument
   must be non‑zero. */
static inline unsigned int v2_uint64(uint64_t x) {
#ifdef __GNUC__
    return __builtin_ctzll(x);
#else
    unsigned int c = 0;
    while ((x & 1ULL) == 0) { x >>= 1; ++c; }
    return c;
#endif
}

seq_t a(idx_t n)
{
    /* Handle the trivial first term */
    if (n == 0) return 1;

    if ((n & 1ULL) == 0) {          /* even n = 2m */
        uint64_t m = n >> 1;           /* m ≥ 1 for n > 0 */
        unsigned int pc = popcount_uint64(m);
        unsigned int v = v2_uint64(m);
        uint64_t exp = 4ULL * m - (1 + v + 2 * pc);
        return pow2(exp);
    } else {                         /* odd n = 2m+1 */
        uint64_t m = (n - 1ULL) >> 1;   /* m ≥ 0 */
        seq_t B = binom(2ULL * m, m);   /* central binomial coefficient */
        unsigned int pc = popcount_uint64(m);
        seq_t Bprime = B >> pc;          /* divide by 2^pc; B is divisible by this */
        seq_t prod  = Bprime * Bprime;   /* (B/2^pc)^2 */
        seq_t result = prod * (seq_t)(2ULL * m + 1ULL);
        return result;
    }
}
