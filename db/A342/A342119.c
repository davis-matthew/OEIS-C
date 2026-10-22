/*
 * A342119  Numbers k with property that if k has m divisors, there are m/2
 *          divisors of k whose sum is k.
 * https://oeis.org/A342119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search‑based algorithm for OEIS A342119: returns the n‑th term.
   a(1) = 2, a(2) = 3, ...
   The algorithm tests each integer in order; it returns the n‑th value k
   that has an even number of divisors and a subset of exactly half of
   those divisors summing to k.  */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/factor_u64.h"   /* uint64_t factor_u64(uint64_t n, uint64_t *, uint64_t *, size_t *); */
#include "../utils/is_prime.h"    /* bool is_prime(uint64_t); */

/* bitwise shift‑or: shift src by 'shift' bits and OR into dst.
   Both src and dst point to arrays of 'words' 64‑bit words. */
static void shift_or(const uint64_t *src, uint64_t *dst,
                     uint64_t shift, size_t words)
{
    const uint64_t word_shift = shift >> 6;      /* shift / 64  */
    const uint32_t bit_shift  = shift & 63;      /* shift % 64  */
    if (word_shift >= words)
        return;
    if (bit_shift == 0) {
        for (size_t i = words; i-- > word_shift; )
            dst[i] |= src[i - word_shift];
    } else {
        for (size_t i = words; i-- > word_shift; ) {
            uint64_t part1 = src[i - word_shift] << bit_shift;
            uint64_t part2 = 0;
            if (i > word_shift)
                part2 = src[i - word_shift - 1] >> (64 - bit_shift);
            dst[i] |= part1 | part2;
        }
        dst[word_shift] |= src[0] << bit_shift;
    }
}

/* Generate all divisors of n from its prime factorisation.
   pr[0..cnt-1] and ex[0..cnt-1] give the primes and exponents. */
static void gen_divs(const uint64_t *pr, const uint64_t *ex,
                     size_t cnt, size_t idx, uint64_t cur,
                     uint64_t *divs, size_t *pos)
{
    if (idx == cnt) {
        divs[(*pos)++] = cur;
        return;
    }
    const uint64_t p = pr[idx];
    uint64_t val = 1;
    for (uint64_t e = 0; e <= ex[idx]; ++e) {
        gen_divs(pr, ex, cnt, idx + 1, cur * val, divs, pos);
        val *= p;
    }
}

/* Return true iff k satisfies the A342119 property. */
static bool isA342119_value(uint64_t k)
{
    if (k < 2)
        return false;

    /* Primes automatically satisfy the property. */
    if (is_prime(k))
        return true;

    /* Factorise k. */
    uint64_t pf[32], ex[32];
    size_t pfcnt = 0;
    factor_u64(k, pf, ex, &pfcnt);

    /* Compute total number of divisors. */
    size_t nd = 1;
    for (size_t i = 0; i < pfcnt; ++i)
        nd *= (ex[i] + 1);

    /* Odd divisor count ⇒ cannot satisfy. */
    if (nd & 1)
        return false;

    const size_t half = nd >> 1;

    /* Generate all divisors. */
    uint64_t *divs = (uint64_t *)malloc(nd * sizeof(uint64_t));
    if (!divs)
        abort();
    size_t pos = 0;
    gen_divs(pf, ex, pfcnt, 0, 1, divs, &pos);

    /* DP: dp[c] is a bitset of attainable sums using exactly c divisors. */
    const uint64_t words = (k >> 6) + 1;
    const size_t dp_words = (half + 1) * words;
    uint64_t *dp = (uint64_t *)calloc(dp_words, sizeof(uint64_t));
    if (!dp) {
        free(divs);
        abort();
    }
    dp[0] = 1ULL;   /* subset of size 0 sums to 0 */

    for (size_t i = 0; i < nd; ++i) {
        const uint64_t d = divs[i];
        for (size_t c = half; c-- > 0; ) {
            const uint64_t *src = dp + c * words;
            uint64_t *dst = dp + (c + 1) * words;
            shift_or(src, dst, d, words);
        }
    }

    const bool ok = ((dp[half * words + (k >> 6)] >> (k & 63)) & 1ULL) != 0;

    free(dp);
    free(divs);
    return ok;
}

seq_t a(idx_t n)
{
    /* search forwards for the n‑th term */
    uint64_t k = 2;          /* start from the first integer >= 2 */
    idx_t found = 0;
    while (true) {
        if (isA342119_value(k)) {
            ++found;
            if (found == n)
                return (seq_t)k;
        }
        ++k;
    }
}
