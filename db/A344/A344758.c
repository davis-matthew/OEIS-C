/*
 * A344758  Smallest divisor d of n for which A011772(d) = A011772(n), where
 *          A011772(n) is the smallest number m such that m(m+1)/2 is
 *          divisible by n.
 * https://oeis.org/A344758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest divisor d of n such that A011772(d)=A011772(n)
 * Range:     u8 n <= 255, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 99 terms through n = 9932 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t compute_t(idx_t d)
{
    /* Find the minimal m such that T_m = m(m+1)/2 is divisible by d.
       Use a running sum to avoid large intermediate values. */
    idx_t mod = 1 % d;
    if (mod == 0) return (seq_t)1;

    /* It is known that a solution always occurs by m <= 2*d.
       This loop guarantees finding it quickly for the required range. */
    for (idx_t m = 2; m <= 2 * d; ++m) {
        mod = (mod + m) % d;
        if (mod == 0) return (seq_t)m;
    }

    /* Fallback: continue indefinitely (should never be reached). */
    for (idx_t m = 2 * d + 1;; ++m) {
        mod = (mod + m) % d;
        if (mod == 0) return (seq_t)m;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but harmless */

    idx_t best_div = n;
    seq_t best_val = compute_t(n);

    /* Enumerate all divisors of n. */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            idx_t d2 = n / i;

            if (d1 != n) {
                seq_t val = compute_t(d1);
                if (val > best_val || (val == best_val && d1 < best_div)) {
                    best_val = val;
                    best_div = d1;
                }
            }

            if (d2 != d1 && d2 != n) {
                seq_t val = compute_t(d2);
                if (val > best_val || (val == best_val && d2 < best_div)) {
                    best_val = val;
                    best_div = d2;
                }
            }
        }
    }

    return (seq_t)best_div;
}
