/*
 * A281116  Number of factorizations of n>=2 into factors greater than 1 with
 *          no common divisor other than 1 (a(1)=0 by convention).
 * https://oeis.org/A281116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered factorizations of n ≥ 2 into factors
 *            > 1 whose overall greatest common divisor is 1
 * Range:     u8 n <= 2627, u16 n <= 8574, u32 n <= 8574, u64 n <= 8574,
 *            u128 n <= 8574, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* integer square‑root (floor) for 0 ≤ n ≤ UINT64_MAX */
static inline idx_t floor_isqrt(idx_t n) {
    if (n < 2) return n;
    idx_t low = 1, high = n >> 1, mid, ans = 1;
    while (low <= high) {
        mid = (low + high) >> 1;
        if (mid <= n / mid) {          /* safe: mid*mid ≤ n without overflow */
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* recursive count of factorizations of n with factors ≥ min_factor.
   g is the gcd of factors chosen so far (0 means no factors yet). */
static seq_t dfs(idx_t n, idx_t min_factor, seq_t g) {
    seq_t count = 0;

    /* finish the factorization with the remaining factor n */
    if (n >= min_factor) {
        seq_t new_g = gcd(g, (seq_t)n);
        if (new_g == 1) {
            count += 1;
        }
    }

    /* try next factor f */
    idx_t limit = floor_isqrt(n);
    for (idx_t f = min_factor; f <= limit; ++f) {
        if ((n % f) == 0) {
            idx_t new_n = n / f;
            seq_t new_g = gcd(g, (seq_t)f);
            if (new_n >= f) {          /* keep non‑decreasing order */
                count += dfs(new_n, f, new_g);
            }
        }
    }
    return count;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;            /* definition a(1)=0; primes give 0 as well */
    return dfs(n, 2, 0);            /* start with no factors chosen (g=0) */
}
