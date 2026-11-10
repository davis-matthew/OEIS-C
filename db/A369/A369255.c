/*
 * A369255  Parity of A140773, where A140773 is the inverse Möbius transform
 *          of A038548.
 * https://oeis.org/A369255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Sum_{m|n} ceil(d(m)/2)) mod 2, where d(m) = number of
 *            divisors of m (A140773(n) mod 2)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 141 terms through n = 9944 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static void A369255_dfs(const uint64_t *exps, size_t idx, size_t cnt,
                        uint64_t d_val, uint64_t *total)
{
    if (idx == cnt) {
        *total += (d_val + 1) >> 1;
        return;
    }
    for (uint64_t f = 0; f <= exps[idx]; ++f) {
        A369255_dfs(exps, idx + 1, cnt, d_val * (f + 1), total);
    }
}

seq_t a(idx_t n)
{
    uint64_t temp = (uint64_t)n;
    uint64_t exps[64];
    size_t cnt = 0;
    uint64_t e = 0;

    while (temp != 0 && (temp & 1ULL) == 0) {
        temp >>= 1;
        ++e;
    }
    if (e) exps[cnt++] = e;

    for (uint64_t p = 3; p <= temp / p; p += 2) {
        if (temp % p == 0) {
            e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            exps[cnt++] = e;
        }
    }
    if (temp > 1) exps[cnt++] = 1;

    uint64_t total = 0;
    A369255_dfs(exps, 0, cnt, 1, &total);
    return (seq_t)(total & 1ULL);
}
