/*
 * A325959  a(n) = n - A325967(n).
 * https://oeis.org/A325959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n - A325967(n), where A325967(n) is found by a subset-sum
 *            DP over the divisors of n
 * Range:     u8 n <= 254, u16 n <= 25000, u32 n <= 25000, u64 n <= 25000,
 *            u128 n <= 25000, big any n
 * Validated: standard, pass, 103 terms through n = 8381 (2026-11-08)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static seq_t A325967_sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static void A325967_gen_divisors_recursive(idx_t idx, idx_t cur,
                                   const uint64_t *pr, const uint64_t *ex,
                                   size_t c, idx_t *out, size_t *cnt) {
    if (idx == c) {
        out[(*cnt)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    idx_t curp = 1;
    for (size_t k = 0; k <= ex[idx]; ++k) {
        A325967_gen_divisors_recursive(idx + 1, cur * curp,
                               pr, ex, c, out, cnt);
        curp *= p;
    }
}

static void A325967_get_divisors(idx_t n, idx_t **out, size_t *cnt) {
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    size_t divcnt = 1;
    for (size_t i = 0; i < pcnt; ++i) divcnt *= (exps[i] + 1);

    *out = (idx_t *)malloc(divcnt * sizeof(idx_t));
    size_t dcnt = 0;
    A325967_gen_divisors_recursive(0, 1, primes, exps, pcnt, *out, &dcnt);
    *cnt = dcnt;
}

static seq_t A325967_a(idx_t n) {
    if (n == 0) return 0;

    idx_t *divs = NULL;
    size_t dcnt = 0;
    A325967_get_divisors(n, &divs, &dcnt);

    seq_t tot = A325967_sigma_of(n);
    idx_t totidx = (idx_t)tot;

    size_t len = totidx / 64 + 1;
    uint64_t *bits = (uint64_t *)calloc(len, sizeof(uint64_t));
    bits[0] = 1ULL;

    for (size_t di = 0; di < dcnt; ++di) {
        idx_t d = divs[di];
        size_t wshift = d >> 6;
        unsigned bshift = (unsigned)(d & 63);
        uint64_t *temp = (uint64_t *)calloc(len, sizeof(uint64_t));

        for (size_t i = 0; i < len; ++i) {
            size_t tgt = i + wshift;
            if (tgt >= len) continue;
            if (bshift == 0) {
                temp[tgt] |= bits[i];
            } else {
                temp[tgt] |= bits[i] << bshift;
                if (tgt + 1 < len)
                    temp[tgt + 1] |= bits[i] >> (64 - bshift);
            }
        }
        for (size_t i = 0; i < len; ++i) bits[i] |= temp[i];
        free(temp);
    }

    seq_t answer = tot;
    idx_t half = totidx / 2;
    for (idx_t s = 0; s <= half; ++s) {
        if ((bits[s >> 6] & (1ULL << (s & 63))) == 0) continue;

        int64_t ia = (int64_t)n - (int64_t)s;
        if (ia < 0) ia = -ia;
        int64_t ib = (int64_t)tot - (int64_t)s - (int64_t)n;
        if (ib < 0) ib = -ib;
        seq_t a1 = (seq_t)ia;
        seq_t b1 = (seq_t)ib;
        if (gcd(a1, b1) == 1) {
            answer = (seq_t)s;
            break;
        }
    }

    free(divs);
    free(bits);
    return answer;
}

seq_t a(idx_t n) {
    return (seq_t)n - A325967_a(n);
}
