/*
 * A337535  For n>1, a(n) is the least base b>2 such that the digits of n in
 *          base b contain the digit b-1; a(1)=1.
 * https://oeis.org/A337535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = min{ b>2 | the base‑b representation of n contains the
 *            digit b-1 }; a(1)=1
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_b_minus_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_b_minus_one.h"

/* Compute floor(sqrt(n)) for idx_t */
static idx_t floor_sqrt(idx_t n) {
    idx_t low = 0, high = n, ans = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= n) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Smallest prime factor of n (n>=2).  Returns n if n is prime. */
static idx_t smallest_prime_factor(idx_t n) {
    if (n % 2 == 0) return 2;
    for (idx_t i = 3;
         (unsigned __int128)i * i <= n;
         i += 2) {
        if (n % i == 0) return i;
    }
    return n;
}

seq_t a(idx_t n) {
    if (n == 1) return 1;

    /* Search bases up to floor(sqrt(n)) */
    idx_t sqrt_n = floor_sqrt(n);
    for (idx_t b = 3; b <= sqrt_n; ++b) {
        if (has_b_minus_one(n, b)) return (seq_t)b;
    }

    /* Candidate b = sqrt_n + 1 (if it satisfies the condition) */
    idx_t cand = sqrt_n + 1;
    if (cand > 2 && has_b_minus_one(n, cand)) return (seq_t)cand;

    /* Use divisors of n+1 for larger bases.
       For any divisor d of n+1, n = k*d + (d-1),
       so the least significant digit is d-1.  */
    idx_t m = n + 1;
    idx_t sp = smallest_prime_factor(m);
    idx_t base_from_divisor = 0;

    if (sp == m) {
        /* n+1 is prime, its only divisor >sqrt_n is itself */
        base_from_divisor = m;
    } else {
        base_from_divisor = m / sp;   /* cofactor, ≥ sqrt(m) */
    }

    if (base_from_divisor > 2) {
        return (seq_t)base_from_divisor;
    } else {
        /* Fallback to the trivial base n+1 */
        return (seq_t)m;
    }
}
