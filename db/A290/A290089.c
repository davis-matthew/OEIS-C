/*
 * A290089  Filter-sequence for the prime signature of cototient: a(1) = 0;
 *          for n > 1, a(n) = A101296(A051953(n)).
 * https://oeis.org/A290089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0; for n > 1, a(n) = A101296(A051953(n)), where
 *            A051953(n) = n - phi(n) and A101296(m) = number of numbers <=
 *            (least number with m's prime signature) whose exponents are
 *            non-increasing over 2,3,5,... (rank of the signature)
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 121 terms through n = 9595 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static const uint64_t A290089_pr[16] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};

static uint64_t A290089_count(uint64_t L, int idx, int maxe, uint64_t cur) {
    uint64_t total = 1;
    if (idx >= 16) return total;
    uint64_t p = A290089_pr[idx];
    uint64_t v = cur;
    for (int e = 1; e <= maxe; ++e) {
        if (v > L / p) break;
        v *= p;
        total += A290089_count(L, idx + 1, e, v);
    }
    return total;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    uint64_t N = (uint64_t)n;
    /* phi(n) */
    uint64_t m = N, ph = N;
    for (uint64_t d = 2; d <= m / d; ++d) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            ph -= ph / d;
        }
    }
    if (m > 1) ph -= ph / m;
    uint64_t c = N - ph;
    /* exponents of c */
    int exps[20];
    int len = 0;
    uint64_t x = c;
    for (uint64_t d = 2; d <= x / d; ++d) {
        if (x % d == 0) {
            int k = 0;
            while (x % d == 0) { x /= d; ++k; }
            exps[len++] = k;
        }
    }
    if (x > 1) exps[len++] = 1;
    /* sort descending */
    for (int i = 1; i < len; ++i) {
        int key = exps[i];
        int j = i;
        while (j > 0 && exps[j-1] < key) { exps[j] = exps[j-1]; --j; }
        exps[j] = key;
    }
    /* least representative */
    uint64_t L = 1;
    for (int i = 0; i < len; ++i)
        for (int e = 0; e < exps[i]; ++e) L *= A290089_pr[i];
    return (seq_t)A290089_count(L, 0, 64, 1);
}
