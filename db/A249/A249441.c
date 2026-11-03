/*
 * A249441  a(n) is the smallest prime whose square divides at least one entry
 *          in the n-th row of Pascal's triangle, or 0 if there is no such
 *          prime.
 * https://oeis.org/A249441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest prime whose square divides at least one entry in
 *            the n-th row of Pascal's triangle, or 0 if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 103 terms through n = 9056 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: number of carries when adding a and b in base 3. */
static unsigned base3_carries(uint64_t a, uint64_t b) {
    unsigned carry = 0;
    unsigned carr_cnt = 0;
    while (a || b || carry) {
        unsigned sum = (unsigned)(a % 3) + (unsigned)(b % 3) + carry;
        if (sum >= 3) {
            ++carr_cnt;
            carry = 1;
            sum -= 3;
        } else {
            carry = 0;
        }
        a /= 3;
        b /= 3;
        if (carr_cnt >= 2) break;
    }
    return carr_cnt;
}

seq_t a(idx_t n)
{
    /* No prime square can divide any entry of row 0,1,2,3. */
    if (n < 4) return 0;

    /* --- check prime 2 --- */
    unsigned long long pop_n = __builtin_popcountll((unsigned long long)n);
    for (idx_t k = 1; k < n; ++k) {
        unsigned long long pop_k   = __builtin_popcountll((unsigned long long)k);
        unsigned long long pop_nk  = __builtin_popcountll((unsigned long long)(n - k));
        if (pop_k + pop_nk >= pop_n + 2) return 2;
    }

    /* --- check prime 3 --- */
    for (idx_t k = 1; k < n; ++k) {
        if (base3_carries(k, n - k) >= 2) return 3;
    }

    /* No prime square divides any binomial coefficient in this row. */
    return 0;
}
