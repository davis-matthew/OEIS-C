/*
 * A340196  a(n) = n - A340195(n).
 * https://oeis.org/A340196
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - Sum_{d|n} gcd(phi(d), d-1) (= Sum_{d|n} A318830(d))
 * Range:     u8 n <= 286, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 106 terms through n = 9985 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

typedef struct {
    uint64_t p;
    int      e;
} A340196_factor_t;

static void A340196_dfs(const A340196_factor_t *fac, int fc, int i,
                        seq_t cur_val, seq_t cur_phi, seq_t *sum)
{
    if (i == fc) {
        *sum += gcd(cur_phi, cur_val - (seq_t)1);
        return;
    }
    uint64_t prime = fac[i].p;
    int exp = fac[i].e;
    seq_t val = cur_val;
    seq_t phi = cur_phi;
    for (int k = 0; k <= exp; ++k) {
        A340196_dfs(fac, fc, i + 1, val, phi, sum);
        if (k < exp) {
            val *= (seq_t)prime;
            if (k == 0)
                phi *= (seq_t)(prime - 1);
            else
                phi *= (seq_t)prime;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    A340196_factor_t fac[20];
    int fc = 0;
    uint64_t tmp = (uint64_t)n;

    if ((tmp & 1) == 0) {
        int e = 0;
        while ((tmp & 1) == 0) {
            tmp >>= 1;
            ++e;
        }
        fac[fc].p = 2;
        fac[fc].e = e;
        ++fc;
    }
    for (uint64_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            fac[fc].p = p;
            fac[fc].e = e;
            ++fc;
        }
    }
    if (tmp > 1) {
        fac[fc].p = tmp;
        fac[fc].e = 1;
        ++fc;
    }

    seq_t sum = 0;
    A340196_dfs(fac, fc, 0, (seq_t)1, (seq_t)1, &sum);
    return (seq_t)n - sum;
}
