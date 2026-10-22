/*
 * A388258  Number of divisors d of n such that d^d == -(-d)^n (mod n).
 * https://oeis.org/A388258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A388258: Number of divisors d of n such that d^d == -(-d)^n (mod n) */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"

/* Integer square root: floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n + 1;
    while (hi - lo > 1) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= n / mid)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Check whether a divisor d of n satisfies the congruence */
static bool check_condition(seq_t d, seq_t mod, idx_t n)
{
    /* left side: d^d mod n */
    seq_t left = mod_pow(d, (uint64_t)d, mod);

    /* right side: -(-d)^n mod n */
    seq_t neg_base = (seq_t)((n - d) % n);          /* (-d) mod n */
    seq_t t = mod_pow(neg_base, (uint64_t)n, mod);  /* (-d)^n mod n */
    seq_t right_neg = (t == 0) ? 0 : mod - t;       /* -t mod n */

    return left == right_neg;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)      /* offset is 1, but guard against zero */
        return 0;

    seq_t count = 0;
    seq_t mod = (seq_t)n;
    idx_t sqrt_n = isqrt_u64(n);

    for (idx_t i = 1; i <= sqrt_n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (check_condition((seq_t)d1, mod, n))
                ++count;

            if (d1 != d2 && check_condition((seq_t)d2, mod, n))
                ++count;
        }
    }

    return count;
}
