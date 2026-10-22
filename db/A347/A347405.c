/*
 * A347405  a(n) = Sum_{d|n} 2^(tau(d) - 1).
 * https://oeis.org/A347405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} 2^(tau(d) - 1)
 * Range:     u8 n <= 35, u16 n <= 179, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static seq_t pow2_seq(idx_t exp) {
    seq_t val = 1;
    for (idx_t i = 0; i < exp; ++i) val *= 2;
    return val;
}

static seq_t dfs(const idx_t *exps, size_t pc, size_t pos, idx_t cur_tau) {
    if (pos == pc) {
        return pow2_seq(cur_tau - 1);
    }
    seq_t sum = 0;
    for (idx_t k = 0; k <= exps[pos]; ++k) {
        sum += dfs(exps, pc, pos + 1, cur_tau * (k + 1));
    }
    return sum;
}

seq_t a(idx_t n) {
    /* factor n into its prime exponents */
    idx_t exps[64];
    size_t pc = 0;
    idx_t m = n;
    if (m < 2) {
        /* n == 1 or 0; but per OEIS offset, n>=1, so n==1 case */
        return 1;
    }
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            exps[pc++] = e;
        }
    }
    if (m > 1) {
        /* remaining prime factor */
        exps[pc++] = 1;
    }
    return dfs(exps, pc, 0, 1);
}
