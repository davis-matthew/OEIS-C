/*
 * A354153  a(n) is the smallest value of a+b+c for nonnegative integers such
 *          that a^b + c = n.
 * https://oeis.org/A354153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the minimum of a+b+c over non‑negative integers a,b,c
 *            such that a^b + c = n. The routine searches all possible bases
 *            a≥2 and exponents b≥2 (the cases a=0,b=0 and a=0,b>0, a=1,b=0/1
 *            are handled by the trivial upper bound n−1)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* compute base^exp, but if the value would exceed limit,
   return limit+1 (a sentinel greater than limit).  This keeps
   the intermediate result within the bounds of seq_t. */
static inline seq_t pow_limit(seq_t base, idx_t exp, seq_t limit)
{
    seq_t result = 1;
    if (base > limit) return limit + 1;   /* any power will exceed limit */
    for (idx_t i = 0; i < exp; ++i) {
        if (result > limit / base) return limit + 1;
        result *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t best = (seq_t)n - 1;          /* 0^0 + (n-1) gives sum n-1 */
    seq_t N     = (seq_t)n;
    /* iterate exponents b >= 2 while 2^b <= n */
    for (idx_t b = 2; ; ++b) {
        if (pow_limit((seq_t)2, b, N) > N) break;   /* 2^b > n -> stop */
        for (idx_t a = 2; ; ++a) {
            seq_t val = pow_limit((seq_t)a, b, N);
            if (val > N) break;                     /* a^b > n -> larger a will also exceed */
            seq_t c   = N - val;
            seq_t sum = a + b + c;
            if (sum < best) best = sum;
        }
    }
    return best;
}
