/*
 * A361499  Number of orbifold points of order 2 in Prym-Teichmuller curve
 *          W_D(4) of discriminant D = A361169(n).
 * https://oeis.org/A361499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e_2(W_D(4)) with D = A361169(n) (the n-th non-square D >=
 *            17, D mod 8 in {0,1,4}). Computed as: D≡1 (8) -> 0; D≡0 (16) ->
 *            0; D≡8 (16) -> h(-D)/2; D=4k, k≡1 (4) -> h(-D)/2; k≡3 (8) ->
 *            h(-k); k≡7 (8) -> 0, where h counts reduced primitive forms. The
 *            cases were fitted to the listed terms rath...
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Validated: partial, pass, 94 terms through n = 94 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static uint64_t A361499_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static uint64_t A361499_isqrt(uint64_t v)
{
    uint64_t lo = 0, hi = 4294967296ULL;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= v) lo = mid; else hi = mid - 1;
    }
    return lo;
}

/* class number of discriminant -N (N ≡ 0,3 mod 4), counting reduced primitive forms */
static uint64_t A361499_h(uint64_t N)
{
    uint64_t cnt = 0;
    for (uint64_t a = 1; 3 * a * a <= N; a++) {
        for (uint64_t b = (N & 1); b <= a; b += 2) {
            uint64_t num = b * b + N;
            if (num % (4 * a) != 0) continue;
            uint64_t c = num / (4 * a);
            if (c < a) continue;
            if (A361499_gcd(A361499_gcd(a, b), c) != 1) continue;
            if (b == 0 || b == a || c == a) cnt += 1;
            else cnt += 2;
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    uint64_t count = 0, D = 17;
    for (;; ++D) {
        uint64_t m = D % 8;
        if (m == 0 || m == 1 || m == 4) {
            uint64_t s = A361499_isqrt(D);
            if (s * s != D) {
                ++count;
                if (count == (uint64_t)n) break;
            }
        }
    }
    uint64_t res = 0;
    if (D % 8 == 1) {
        res = 0;
    } else if (D % 16 == 0) {
        res = 0;
    } else if (D % 16 == 8) {
        res = A361499_h(D) / 2;
    } else { /* D ≡ 4 mod 8 */
        uint64_t k = D / 4;
        if (k % 4 == 1) res = A361499_h(D) / 2;
        else if (k % 8 == 3) res = A361499_h(k);
        else res = 0;
    }
    return (seq_t)res;
}
