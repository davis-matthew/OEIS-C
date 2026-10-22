/*
 * A385608  a(n) = 2-adic valuation of A003266(n).
 * https://oeis.org/A385608
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*floor(n/3) + floor(n/6) - popcount(floor(n/3))
 * Range:     u8 n <= 311, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where popcount counts the 1's in the binary representation.
   This implements the OEIS definition of the 2‑adic valuation of A003266(n). */

#include <stdint.h>

/* Helper: population count for uint64_t (assumes compiler supports __builtin_popcountll) */
static inline unsigned int popcnt_u64(uint64_t x)
{
#if defined(__GNUC__) || defined(__clang__) || defined(_MSC_VER)
    return __builtin_popcountll(x);
#else
    /* Fallback implementation (slow but correct) */
    unsigned int c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
#endif
}

seq_t a(idx_t n)
{
    /* floor(n/3) and floor(n/6) */
    idx_t m = n / 3;
    idx_t k = n / 6;

    /* Compute 2*m + k as idx_t, then cast to seq_t */
    seq_t val = (seq_t)(2 * m + k);

    /* Subtract the popcount of m */
    unsigned int pc = popcnt_u64((uint64_t)m);
    return val - (seq_t)pc;
}
