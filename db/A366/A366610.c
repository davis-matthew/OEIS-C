/*
 * A366610  Number of successive occurrences of the same first digits in
 *          A366585.
 * https://oeis.org/A366610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   simulate A366585 one leading-digit block at a time. Groups start
 *            at s with step d, where d = first digit of s and p =
 *            10^(len(s)-1). Keep s as d*p + o with 0 <= o < p. The run for
 *            digit d has a(n) = ceil((p - o)/d) terms. After the run, o' = o
 *            + a(n)*d - p and d moves to d+1; after digit 9, p bec...
 * Range:     u8 n <= 22, u16 n <= 40, u32 n <= 85, u64 n <= 175,
 *            u128 n <= 346, big any n
 * Validated: standard, pass, 72 terms through n = 1948 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t p = 1;      /* 10^(k-1) for the current digit length k */
    seq_t o = 0;      /* offset of the current group start inside block d*p */
    int d = 1;        /* current leading digit */
    idx_t i = 0;

    for (;;) {
        /* skip leading digits whose block contains no group start */
        while (o >= p) {
            o -= p;
            d++;
            if (d == 10) { p *= 10; d = 1; }
        }
        seq_t count = (p - o + (d - 1)) / d;
        i++;
        if (i == n) return count;
        o = o + count * d - p;     /* 0 <= o < d */
        d++;
        if (d == 10) { p *= 10; d = 1; }
    }
}
