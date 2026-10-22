/*
 * A325967  a(n) is the minimum sum of all such subsets of divisors of n for
 *          which n-s and (sigma(n)-s)-n are relatively prime, where s is the
 *          sum of the subset.
 * https://oeis.org/A325967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 392, u16 n <= 9131, u32 n <= 9131, u64 n <= 9131,
 *            u128 n <= 9131, big any n
 * Helpers:   factor_u64, gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = min{ s | s is the sum of a subset of the divisors of n,
 *                     gcd(n-s, sigma(n)-s-n) = 1 }.
 * Implementation: subset‑sum dynamic programming over the divisor set.
 * The algorithm is O(τ(n)·σ(n)/64) and fits easily in time for n ≤ 10 000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/gcd.h"
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"

static void gen_divisors_recursive(idx_t idx, idx_t cur,
                                   const uint64_t *pr, const uint64_t *ex,
                                   size_t c, idx_t *out, size_t *cnt) {
    if (idx == c) {
        out[(*cnt)++] = cur;
        return;
    }
    uint64_t p = pr[idx];
    idx_t curp = 1;
    for (size_t k = 0; k <= ex[idx]; ++k) {
        gen_divisors_recursive(idx + 1, cur * curp,
                               pr, ex, c, out, cnt);
        curp *= p;
    }
}

static void get_divisors(idx_t n, idx_t **out, size_t *cnt) {
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64((uint64_t)n, primes, exps, &pcnt);

    size_t divcnt = 1;
    for (size_t i = 0; i < pcnt; ++i) divcnt *= (exps[i] + 1);

    *out = (idx_t *)malloc(divcnt * sizeof(idx_t));
    size_t dcnt = 0;
    gen_divisors_recursive(0, 1, primes, exps, pcnt, *out, &dcnt);
    *cnt = dcnt;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;          /* safety for 0 index, unused */

    /* Divisors of n */
    idx_t *divs = NULL;
    size_t dcnt = 0;
    get_divisors(n, &divs, &dcnt);

    /* Total sum of divisors σ(n) */
    seq_t tot = sigma_of(n);
    idx_t totidx = (idx_t)tot;     /* σ(n) fits in idx_t for n ≤ 10 000 */

    /* Bitset DP for reachable subset sums up to σ(n) */
    size_t len = totidx / 64 + 1;
    uint64_t *bits = (uint64_t *)calloc(len, sizeof(uint64_t));
    bits[0] = 1ULL;                 /* sum 0 is possible */

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

    /* Find minimal s with the gcd condition. */
    seq_t answer = tot;                 /* default, unlikely to be used */
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
