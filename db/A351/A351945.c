/*
 * A351945  a(n) = A342001(A181819(n)).
 * https://oeis.org/A351945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A342001(A181819(n)); with m = prod prime(e_i) for n =
 *            prod p_i^e_i, a(n) = sum over distinct primes q | m of v_q(m) *
 *            rad(m)/q
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9914 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const unsigned A351945_pe[65] = {
    0, 2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
    31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
    127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
    179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
    233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
    283, 293, 307, 311
};

seq_t a(idx_t n)
{
    unsigned cnt[65];
    int i;
    for (i = 0; i < 65; ++i) cnt[i] = 0;

    uint64_t t = (uint64_t)n;
    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int e = 0;
            while (t % p == 0) { t /= p; ++e; }
            cnt[e]++;
        }
    }
    if (t > 1) cnt[1]++;

    seq_t rad = 1;
    for (i = 1; i < 65; ++i)
        if (cnt[i]) rad *= (seq_t)A351945_pe[i];

    seq_t res = 0;
    for (i = 1; i < 65; ++i)
        if (cnt[i])
            res += (seq_t)cnt[i] * (rad / (seq_t)A351945_pe[i]);
    return res;
}
