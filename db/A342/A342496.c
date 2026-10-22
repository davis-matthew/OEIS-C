/*
 * A342496  Number of integer partitions of n with constant (equal) first
 *          quotients.
 * https://oeis.org/A342496
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd_uint64, geom_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A342496: Number of integer partitions of n with constant (equal) first quotients.
   a(0) = 1
   a(n) for n>0: 1 (single part) + ⌊n/2⌋ (two‑part partitions) +
                 (τ(n)–1 – [n even]) (equal‑part partitions with ≥3 parts) +
                 (number of geometric progressions with ratio p/q (1≤p<q, gcd=1)
                   and length k≥3 such that n = t·∑_{i=0}^{k-1} p^i q^{k-1-i})
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"
#include "../utils/geom_sum.h"

/* gcd for 64‑bit unsigned integers */

/* Number of divisors τ(n) by trial division */
static inline uint64_t divisor_count_uint64(uint64_t n) {
    if (n == 0) return 0;
    uint64_t res = 1;
    uint64_t d = 2;
    while (d * d <= n) {
        if (n % d == 0) {
            uint64_t exp = 0;
            while (n % d == 0) {
                n /= d;
                ++exp;
            }
            res *= (exp + 1);
        }
        d += (d == 2) ? 1 : 2;
    }
    if (n > 1)
        res *= 2;
    return res;
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;                     /* the empty partition */

    const uint64_t N = (uint64_t)n;
    uint64_t cnt = 0;

    /* single part */
    cnt += 1;

    /* two‑part partitions: ⌊n/2⌋ */
    cnt += N / 2;

    /* equal‑part partitions with ≥3 parts:
       τ(n) – 1 (exclude k = 1) minus 1 if n even (k=2 already counted) */
    uint64_t tau = divisor_count_uint64(N);
    uint64_t equal_cnt = tau - 1;
    if ((N & 1ULL) == 0)       /* n even → k = 2 partition already counted */
        equal_cnt -= 1;
    if (equal_cnt > 0)
        cnt += equal_cnt;

    /* geometric progressions with length k ≥ 3 and ratio p/q in lowest terms */
    for (uint64_t k = 3;; ++k) {
        /* minimal sum for this k is 2^k – 1 (p = 1, q = 2) */
        uint64_t minT = 1;
        for (uint64_t i = 0; i < k - 1; ++i)
            minT = minT * 2 + 1;          /* 2^k – 1 */
        if (minT > N)
            break;                       /* no larger k will fit */

        for (uint64_t p = 1;; ++p) {
            /* minimal q for this p is p + 1 */
            uint64_t T_min = geom_sum(p, p + 1, k, N);
            if (T_min > N)
                break;                   /* larger p will only increase T_min */

            for (uint64_t q = p + 1;; ++q) {
                if (gcd_uint64(p, q) != 1)
                    continue;           /* use reduced ratio only */

                uint64_t T = geom_sum(p, q, k, N);
                if (T > N)
                    break;               /* further q will only increase T */

                if (N % T == 0)
                    ++cnt;
            }
        }
    }

    return (seq_t)cnt;
}
