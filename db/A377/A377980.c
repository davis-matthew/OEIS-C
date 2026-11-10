/*
 * A377980  Lexicographically earliest sequence of distinct nonnegative
 *          integers such that for any n >= 0, the binary expansions of
 *          A184616(n) and A184616(a(n)) have no common 1's.
 * https://oeis.org/A377980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   greedy search. For n = 0, 1, 2, ..., if a(n) is not yet set, let
 *            a(n) be the least unused m >= n with A184616(n) AND A184616(m) =
 *            0, and set a(m) = n (self-inverse)
 * Range:     u8 n <= 212, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 84 terms through n = 7604 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t A184616_b(uint64_t m) {
    uint64_t sum = 0, power = 1;
    while (m > 0) {
        if (m & 1) {
            if ((m & 3) == 1) {
                m = (m - 1) >> 1;
            } else {
                sum += power;
                m = (m + 1) >> 1;
            }
        } else {
            m >>= 1;
        }
        power <<= 1;
    }
    return sum;
}

static uint64_t *A377980_tab = 0;
static uint64_t A377980_cap = 0;
static int A377980_init = 0;

static void A377980_build(uint64_t P) {
    uint64_t i, n, m;
    uint64_t *t = (uint64_t *)malloc((P + 1) * sizeof(uint64_t));
    uint64_t *b = (uint64_t *)malloc((P + 1) * sizeof(uint64_t));
    for (i = 0; i <= P; i++) {
        t[i] = UINT64_MAX;
        b[i] = A184616_b(i);
    }
    for (n = 0; n <= P; n++) {
        if (t[n] != UINT64_MAX) continue;
        for (m = n; m <= P; m++) {
            if (t[m] == UINT64_MAX && (b[n] & b[m]) == 0) {
                t[n] = m;
                t[m] = n;
                break;
            }
        }
        if (t[n] == UINT64_MAX) t[n] = n; /* fallback, should not occur */
    }
    free(b);
    if (A377980_tab) free(A377980_tab);
    A377980_tab = t;
    A377980_cap = P;
    A377980_init = 1;
}

seq_t a(idx_t n) {
    uint64_t un = (uint64_t)n;
    if (!A377980_init || un > A377980_cap) {
        uint64_t P = 1;
        while (P < un) P <<= 1;
        A377980_build(P);
    }
    return (seq_t)A377980_tab[un];
}
