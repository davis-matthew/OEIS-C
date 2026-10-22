/*
 * A375706  First differences of non-perfect-powers.
 * https://oeis.org/A375706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007916(n+1) - A007916(n) – first differences of
 *            non‑perfect‑powers Computation: iterate over integers, skip
 *            those that are perfect powers, and track the nth and (n+1)‑st
 *            non‑perfect‑powers
 * Range:     u8 n <= 9124, u16 n <= 9124, u32 n <= 9124, u64 n <= 9124,
 *            u128 n <= 9124, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Test whether n ≥ 2 is a perfect power: there exists b ≥ 2, e ≥ 2
   such that n = b^e.  The search is limited to b^2 ≤ n, which is
   sufficient for all n that fit in 64 bits. */
static bool is_perfect_power(idx_t n) {
    if (n < 4) return false;          /* 2 and 3 are not perfect powers */
    for (idx_t base = 2; base * base <= n; ++base) {
        idx_t val = base * base;
        while (val <= n) {
            if (val == n) return true;
            if (val > n / base) break;  /* next multiplication would overflow or exceed n */
            val *= base;
        }
    }
    return false;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;  /* undefined for n = 0; return 0 just in case */

    idx_t count = 0;
    idx_t prev = 0, curr = 0;
    /* start from 2, the first non‑perfect‑power (A007916(1)) */
    for (idx_t m = 2; ; ++m) {
        if (!is_perfect_power(m)) {
            ++count;
            if (count == n) prev = m;
            if (count == n + 1) { curr = m; break; }
        }
    }
    return (seq_t)(curr - prev);
}
