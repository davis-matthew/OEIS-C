/*
 * A283498  a(n) = Sum_{d|n} d^(d+1).
 * https://oeis.org/A283498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d^(d+1) (OEIS A283498)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) using binary search and 128‑bit math
   when available.  This works for all idx_t values. */
static inline idx_t int_sqrt(idx_t n) {
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
#ifdef __SIZEOF_INT128__
        __uint128_t m2 = (__uint128_t)mid * mid;
        if (m2 <= n) { ans = mid; low = mid + 1; }
        else high = mid - 1;
#else
        /* Fallback for compilers without 128‑bit integers: use standard
           double sqrt (safe for n <= 4·10^18). */
        if ((mid > 0 && mid > n / mid) || (mid * mid > n))
            high = mid - 1;
        else {
            ans = mid;
            low = mid + 1;
        }
#endif
    }
    return ans;
}

/* Fast exponentiation by squaring.  Uses only seq_t multiplication,
   which is safe for arbitrary‑precision types. */
static seq_t power_seq(seq_t base, idx_t exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* sequence defined for n ≥ 1, but guard */
    seq_t sum = 0;
    idx_t r = int_sqrt(n);
    for (idx_t i = 1; i <= r; ++i) {
        if (n % i == 0) {
            seq_t d1 = (seq_t)i;
            sum += power_seq(d1, (idx_t)(i + 1));
            idx_t j = n / i;
            if (j != i) {
                seq_t d2 = (seq_t)j;
                sum += power_seq(d2, (idx_t)(j + 1));
            }
        }
    }
    return sum;
}
