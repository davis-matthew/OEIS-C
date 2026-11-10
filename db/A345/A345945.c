/*
 * A345945  Numbers k that have such a proper divisor d for which A002034(d) =
 *          A002034(k).
 * https://oeis.org/A345945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff A002034(k/p) = A002034(k) for some prime
 *            p | k (A002034 is monotone under divisibility, so checking
 *            maximal proper divisors suffices)
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 79 terms through n = 79 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A002034_v_p(seq_t m, seq_t p) {
    seq_t result = 0;
    seq_t power = p;
    while (power <= m) {
        result += m / power;
        if (power > m / p) break;
        power *= p;
    }
    return result;
}

static seq_t A002034_min_m(seq_t p, seq_t k, seq_t upper_bound) {
    seq_t lo = 0, hi = upper_bound;
    while (lo < hi) {
        seq_t mid = lo + (hi - lo) / 2;
        if (A002034_v_p(mid, p) >= k) hi = mid;
        else lo = mid + 1;
    }
    return lo;
}

static seq_t A002034_a(seq_t n) {
    if (n == 1) return 1;
    seq_t result = 0;
    seq_t temp = n;
    for (seq_t p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            seq_t k = 0;
            while (temp % p == 0) {
                k++;
                temp /= p;
            }
            seq_t m = A002034_min_m(p, k, n);
            if (m > result) result = m;
        }
    }
    if (temp > 1) {
        if (temp > result) result = temp;
    }
    return result;
}

static bool A345945_is_member(seq_t k) {
    seq_t s = A002034_a(k);
    seq_t temp = k;
    for (seq_t p = 2; p <= temp / p; p++) {
        if (temp % p == 0) {
            while (temp % p == 0) temp /= p;
            if (A002034_a(k / p) == s) return true;
        }
    }
    if (temp > 1) {
        if (A002034_a(k / temp) == s) return true;
    }
    return false;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t k = 1;
    while (count < n) {
        k++;
        if (A345945_is_member(k)) count++;
    }
    return k;
}
