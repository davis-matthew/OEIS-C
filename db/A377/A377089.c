/*
 * A377089  Numbers that are both happy (A007770) and elated (A376272).
 * https://oeis.org/A377089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th number that is both happy (A007770) and elated
 *            (A376272); computed by a search with cycle detection on each map
 * Range:     u8 n <= 3, u16 n <= 1045, u32 n <= 12205, u64 n <= 12205,
 *            u128 n <= 12205, big any n
 * Validated: standard, pass, 63 terms through n = 9974 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t A003132_ss(seq_t num) {
    seq_t sum = 0;
    while (num > 0) {
        seq_t digit = num % 10;
        sum += digit * digit;
        num /= 10;
    }
    return sum;
}

static bool A007770_is_happy(seq_t num) {
    seq_t slow = num;
    seq_t fast = num;
    while (1) {
        slow = A003132_ss(slow);
        if (slow == 1) return true;
        fast = A003132_ss(fast);
        if (fast == 1) return true;
        fast = A003132_ss(fast);
        if (fast == 1) return true;
        if (slow == fast) return false;
    }
}

/* A376270 map: leading digit times sum of squares of digits */
static seq_t A376270_f(seq_t n) {
    seq_t s = A003132_ss(n);
    seq_t lead = n;
    while (lead >= 10) lead /= 10;
    return lead * s;
}

static bool A376272_is_elated(seq_t num) {
    seq_t slow = num;
    seq_t fast = num;
    while (1) {
        slow = A376270_f(slow);
        if (slow == 1) return true;
        fast = A376270_f(fast);
        if (fast == 1) return true;
        fast = A376270_f(fast);
        if (fast == 1) return true;
        if (slow == fast) return false;
    }
}

static idx_t cache_count = 0;
static seq_t cache_val = 0;

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 0;
    if (cache_count != 0 && cache_count <= n) {
        count = cache_count;
        candidate = cache_val;
    }
    while (count < n) {
        candidate++;
        if (A007770_is_happy(candidate) && A376272_is_elated(candidate)) {
            count++;
        }
    }
    cache_count = count;
    cache_val = candidate;
    return candidate;
}
