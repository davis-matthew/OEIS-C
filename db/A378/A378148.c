/*
 * A378148  a(n) is the number of distinct trapezoids having integer sides and
 *          height with exactly one pair of parallel sides and area n.
 * https://oeis.org/A378148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_int32, factor_h, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A378148
   a(n) = number of distinct trapezoids with integer sides and height
   having exactly one pair of parallel sides and area n.
   Returned value is a 0‑based count; the sequence is indexed from 1. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */
#include "../utils/cmp_int32.h"
#include "../utils/factor_h.h"

/* recursively generate all divisors of h² */
static void gen_divisors_rec(const uint64_t *primes,
                            const int *exps,
                            size_t depth,
                            size_t prime_cnt,
                            uint64_t cur,
                            uint64_t *divs,
                            size_t *cnt)
{
    if (depth == prime_cnt) {
        divs[(*cnt)++] = cur;
        return;
    }
    uint64_t p = primes[depth];
    uint64_t val = cur;
    for (int e = 0; e <= 2 * exps[depth]; ++e) {
        gen_divisors_rec(primes, exps, depth + 1, prime_cnt,
                         val, divs, cnt);
        val *= p;
    }
}

seq_t a(idx_t n)
{
    /* iterate over all divisors h of 2*n */
    const uint64_t twice_n = (uint64_t)2 * (uint64_t)n;
    seq_t total = 0;

    /* maximum possible offset size: 2*n (for h=1) */
    int32_t offsets[8192];
    size_t offs_cnt = 0;

    for (uint64_t d = 1; d * d <= twice_n; ++d) {
        if (twice_n % d != 0)
            continue;
        uint64_t h_list[2] = { d, twice_n / d };
        for (size_t hh = 0; hh < 2; ++hh) {
            uint64_t h = h_list[hh];
            /* avoid duplicate when d == twice_n / d */
            if (hh == 1 && d == twice_n / d)
                continue;

            /* s = p + q = 2*n / h */
            uint64_t s = twice_n / h;
            if (s < 2)
                continue;   /* p+q must be at least 2 */

            uint64_t offset_limit = (s >= 2) ? s - 2 : 0ULL;   /* t <= s-2 */
            /* generate all a such that a^2 + h^2 is a perfect square */
            uint64_t h2 = h * h;

            /* factorise h */
            uint64_t primes[20];
            int exps[20];
            size_t prime_cnt = 0;
            factor_h(h, primes, exps, &prime_cnt);

            /* enumerate divisors of h2 */
            uint64_t divs[5000];   /* ample capacity for h <= 20000 */
            size_t div_cnt = 0;
            gen_divisors_rec(primes, exps, 0, prime_cnt,
                             1ULL, divs, &div_cnt);

            offs_cnt = 0;
            for (size_t i = 0; i < div_cnt; ++i) {
                uint64_t u = divs[i];
                uint64_t v = h2 / u;
                if (u > v)
                    continue;
                if (((u + v) & 1ULL) != 0ULL)   /* parity mismatch */
                    continue;
                uint64_t a_pos = (v - u) >> 1;
                if (a_pos > INT32_MAX)
                    continue;   /* safety, should not happen for h <= 20000 */
                int32_t a_val = (int32_t)a_pos;
                if (offs_cnt < sizeof(offsets) / sizeof(offsets[0])) {
                    offsets[offs_cnt++] = a_val;
                    if (a_pos > 0) {
                        int32_t neg = -a_val;
                        if (offs_cnt < sizeof(offsets) / sizeof(offsets[0]))
                            offsets[offs_cnt++] = neg;
                    }
                }
            }

            /* unique and sort offsets */
            qsort(offsets, offs_cnt, sizeof(int32_t), cmp_int32);
            size_t uniq_cnt = 0;
            for (size_t i = 0; i < offs_cnt; ++i) {
                if (i == 0 || offsets[i] != offsets[i-1])
                    offsets[uniq_cnt++] = offsets[i];
            }
            offsets[uniq_cnt] = 0; /* sentinel not used */

            /* count pairs */
            for (size_t i = 0; i < uniq_cnt; ++i) {
                int32_t a_off = offsets[i];
                for (size_t j = i; j < uniq_cnt; ++j) {
                    int32_t b_off = offsets[j];
                    int64_t t = (int64_t)a_off + (int64_t)b_off;
                    if (t <= 0 || (uint64_t)t > offset_limit)
                        continue;
                    if ( ((t & 1LL) != (s & 1ULL)) )
                        continue;   /* parity must match */

                    /* compute p and q */
                    seq_t p = (seq_t)(s + t) / 2;
                    seq_t q = (seq_t)(s - t) / 2;
                    if (q < 1)
                        continue;   /* safety, should not happen */

                    /* ensure that the side lengths are integers (they are by construction) */
                    ++total;
                }
            }
        }
    }
    return total;
}
