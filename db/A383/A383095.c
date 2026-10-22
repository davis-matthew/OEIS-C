/*
 * A383095  Number of integer partitions of n having exactly one permutation
 *          with all equal run-sums.
 * https://oeis.org/A383095
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer partitions of n having exactly one
 *            permutation ** with all equal run-sums
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Return gcd of two non‑negative integers. */
static inline idx_t _gcd(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Check whether a partition consisting of two distinct part values
** a and b with multiplicities f_a and f_b has exactly one permutation
** with all equal run‑sums.  The partition is represented by the
** unordered pair {a,b} with a < b. */
static inline int _has_unique_equal_runs(idx_t a, idx_t b,
                                         idx_t f_a, idx_t f_b)
{
    idx_t g   = _gcd(a, b);
    idx_t a1  = a / g;          /* reduced values, gcd(a1,b1)=1 */
    idx_t b1  = b / g;

    /* Difference needed for the unique‑run condition. */
    long long diff_ll = (long long)f_a * a1 - (long long)f_b * b1;
    if (diff_ll < 0) diff_ll = -diff_ll;
    if (diff_ll == 0) return 0;                 /* r_a == r_b, not counted */

    /* Check divisibility of diff by a1*b1. */
    unsigned long long ab1 = (unsigned long long)a1 * b1;
    if (diff_ll % ab1 != 0) return 0;
    unsigned long long t = diff_ll / ab1;
    if (t == 0) return 0;

    /* Verify divisibility conditions for the multiplicities. */
    if (f_a % (b1 * t) != 0) return 0;
    if (f_b % (a1 * t) != 0) return 0;

    /* The difference between run counts is exactly 1, giving a unique
       arrangement of runs. */
    return 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Special case for n = 0: the empty partition counts once. */
    if (n == 0) return 1;

    /* Count partitions with all parts equal: one for each divisor of n. */
    idx_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* divisor d */
            count++;
            /* partner divisor n/d, avoid double counting for perfect squares. */
            if (d * d != n) count++;
        }
    }

    /* Count partitions with exactly two distinct part values. */
    for (idx_t a = 1; a < n; ++a) {
        idx_t max_fa = n / a - 1;          /* f_a must leave a positive remainder */
        for (idx_t f_a = 1; f_a <= max_fa; ++f_a) {
            idx_t rem = n - a * f_a;       /* remaining sum to be split between b's */
            /* Enumerate divisors of rem to obtain the second part value b. */
            for (idx_t d = 1; d * d <= rem; ++d) {
                if (rem % d != 0) continue;
                /* First divisor pair (b = d, f_b = rem/d) */
                idx_t b1 = d;
                idx_t f_b1 = rem / d;
                if (b1 != a && a < b1 &&
                    _has_unique_equal_runs(a, b1, f_a, f_b1))
                    ++count;

                /* Second divisor pair when rem is not a perfect square. */
                if (d * d == rem) continue;
                idx_t b2 = rem / d;
                idx_t f_b2 = d;
                if (b2 != a && a < b2 &&
                    _has_unique_equal_runs(a, b2, f_a, f_b2))
                    ++count;
            }
        }
    }

    return (seq_t)count;
}
