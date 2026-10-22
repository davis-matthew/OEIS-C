/*
 * A363850  Number of divisors of 7*n-3 of form 7*k+1.
 * https://oeis.org/A363850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors d of 7*n-3 with d ≡ 1 (mod 7)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;            // offset is 1, but guard against zero
    uint64_t m = 7ull * (uint64_t)n - 3;   /* 7*n-3 fits in uint64_t for test data */

    /* Factor m */
    uint64_t tmp = m;
    uint64_t primes[32];
    int    exps  [32];
    int    fcnt  = 0;
    for (uint64_t d = 2; d * d <= tmp; ++d) {
        if (tmp % d == 0) {
            int e = 0;
            while (tmp % d == 0) {
                tmp /= d;
                ++e;
            }
            primes[fcnt] = d;
            exps  [fcnt] = e;
            ++fcnt;
        }
    }
    if (tmp > 1) {              /* remaining prime factor */
        primes[fcnt] = tmp;
        exps  [fcnt] = 1;
        ++fcnt;
    }

    /* Generate all divisors of m */
    uint64_t divisors[8192];
    int div_cnt = 1;
    divisors[0] = 1;
    for (int i = 0; i < fcnt; ++i) {
        uint64_t p = primes[i];
        int e      = exps[i];
        int cur_sz = div_cnt;
        for (int j = 0; j < cur_sz; ++j) {
            uint64_t base = divisors[j];
            for (int k = 1; k <= e; ++k) {
                base *= p;
                divisors[div_cnt++] = base;
            }
        }
    }

    /* Count divisors that are ≡ 1 (mod 7) */
    seq_t cnt = 0;
    for (int i = 0; i < div_cnt; ++i) {
        if (divisors[i] % 7 == 1) ++cnt;
    }
    return cnt;
}
