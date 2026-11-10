/*
 * A344975  Numbers k such that A011772(k) divides A344875(k), but k is not a
 *          power of prime (in A000961).
 * https://oeis.org/A344975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (k not 1 or a prime power) such that A011772(k)
 *            divides A344875(k); found by direct search
 * Range:     u8 n <= 54, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A344975_t(uint64_t k)
{
    for (uint64_t m = 1; m < 2 * k; ++m) {
        if (((m * (m + 1)) / 2) % k == 0)
            return m;
    }
    return 2 * k - 1;
}

/* returns A344875(k); sets *npf to number of distinct primes */
static uint64_t A344975_s(uint64_t k, int *npf)
{
    uint64_t ans = 1;
    uint64_t tmp = k;
    int cnt = 0;
    for (uint64_t p = 2; tmp > 1; p += (p == 2 ? 1 : 2)) {
        if (p * p > tmp) p = tmp;
        if (tmp % p == 0) {
            uint64_t pw = 1;
            while (tmp % p == 0) {
                tmp /= p;
                pw *= p;
            }
            ++cnt;
            if (p == 2) ans *= (2 * pw - 1);
            else ans *= (pw - 1);
        }
    }
    *npf = cnt;
    return ans;
}

seq_t a(idx_t n)
{
    uint64_t count = 0;
    for (uint64_t k = 2;; ++k) {
        int npf = 0;
        uint64_t s = A344975_s(k, &npf);
        if (npf < 2) continue;
        uint64_t t = A344975_t(k);
        if (s % t == 0) {
            ++count;
            if (count == (uint64_t)n)
                return (seq_t)k;
        }
    }
}
