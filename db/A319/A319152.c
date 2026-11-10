/*
 * A319152  Nonprime Heinz numbers of superperiodic integer partitions.
 * https://oeis.org/A319152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th number m (a search over perfect powers A001597) with
 *            m != 2 and m in S, where S: 2 in S; m in S iff the prime indices
 *            of m have gcd > 1 and A181819(m) in S (all primes are in S)
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-08)
 * Helpers:   factor_u64, gcd_u64, is_prime_u64, mobius_i32, nth_prime_u64, prime_pi_u64
 *            prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mobius_i32.h"
#include "../utils/factor_u64.h"
#include "../utils/is_prime_u64.h"
#include "../utils/prime_pi_u64.h"
#include "../utils/nth_prime_u64.h"
#include "../utils/gcd_u64.h"

static bool A001597_pow_le(uint64_t b, int k, uint64_t x) {
    uint64_t r = 1;
    for (int i = 0; i < k; i++) {
        if (r > x / b) return false;
        r *= b;
    }
    return true;
}

static uint64_t A001597_iroot(uint64_t x, int k) {
    int sh = 64 / k + 1;
    uint64_t lo = 1, hi = (sh >= 63) ? ((uint64_t)1 << 33) : ((uint64_t)1 << sh);
    if (x == 0) return 0;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (A001597_pow_le(mid, k, x)) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static uint64_t A001597_count(uint64_t x) {
    long long c = 1;
    for (int k = 2; k < 64; k++) {
        uint64_t r = A001597_iroot(x, k);
        if (r < 2) break;
        c -= (long long)mobius_i32(k) * (long long)(r - 1);
    }
    return (uint64_t)c;
}

static uint64_t A001597_a64(uint64_t n) {
    if (n <= 1) return 1;
    uint64_t lo = 1, hi = n * n;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo) / 2;
        if (A001597_count(mid) >= n) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

static bool a319152_inS(uint64_t m) {
    if (m == 1) return false;
    if (m == 2) return true;
    if (is_prime_u64(m)) return true;
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(m, pr, ex, &cnt);
    uint64_t g = 0;
    for (size_t i = 0; i < cnt; i++) {
        g = gcd_u64(g, prime_pi_u64(pr[i]));
        if (g == 1) return false;
    }
    uint64_t sh = 1;
    for (size_t i = 0; i < cnt; i++) sh *= nth_prime_u64(ex[i]);
    return a319152_inS(sh);
}

seq_t a(idx_t n) {
    static uint64_t *terms = NULL;
    static size_t cnt = 0, cap = 0;
    static uint64_t k = 1; /* last perfect-power index examined */
    while (cnt < (size_t)n) {
        k++;
        uint64_t m = A001597_a64(k);
        if (m == 1 || m == 2) continue;
        if (a319152_inS(m)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 64;
                terms = (uint64_t *)realloc(terms, cap * sizeof(uint64_t));
                if (!terms) abort();
            }
            terms[cnt++] = m;
        }
    }
    return (seq_t)terms[(size_t)n - 1];
}
