/*
 * A392595  a(n) = A324895(n) mod 360, where A324895 gives the largest proper
 *          divisor of primorial base exp-function.
 * https://oeis.org/A392595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A324895(n) mod 360, where A324895(n) is the largest
 *            proper divisor of the primorial base exp-function of n
 * Range:     u8 n <= 24, u16 n <= 9738, u32 n <= 9738, u64 n <= 9738,
 *            u128 n <= 9738, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

static const idx_t _primes[] = {
    2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,
    31ULL,37ULL,41ULL,43ULL,47ULL,53ULL,59ULL,61ULL,67ULL,71ULL
};
static const size_t _primeCnt = sizeof(_primes)/sizeof(_primes[0]);

seq_t a(idx_t n)
{
    /* ============= obtain primorial powers ≤ n ============= */
    /* w[i] = product_{k=0}^{i-1} _primes[k], w[0] = 1          */
    idx_t w[64];
    size_t digitCnt = 1;              /* w[0] = 1 is always present */
    w[0] = 1;
    while (digitCnt < 64 && digitCnt < _primeCnt) {
        /* next primorial is w[digitCnt] = w[digitCnt-1] * _primes[digitCnt-1]   */
        if (w[digitCnt-1] > n / _primes[digitCnt-1]) break; /* would exceed n */
        w[digitCnt] = w[digitCnt-1] * _primes[digitCnt-1];
        digitCnt++;
    }

    /* ============= compute primorial base digits ============= */
    uint64_t digits[64] = {0};
    idx_t rem = n;
    for (int i = (int)digitCnt - 1; i >= 0; --i) {
        digits[i] = rem / w[i];
        rem %= w[i];
    }

    /* ============= find smallest i with non‑zero digit ============== */
    size_t minIdx = digitCnt;        /* sentinel if none found (n == 0) */
    for (size_t i = 0; i < digitCnt; ++i) {
        if (digits[i] > 0) { minIdx = i; break; }
    }

    /* ============= compute product of prime powers with one exponent reduced ============= */
    uint64_t acc = 1ULL;   /* we perform mod 360 in a wider type            */
    for (size_t i = 0; i < digitCnt; ++i) {
        uint64_t exp = digits[i];
        if (exp == 0) continue;
        if (i == minIdx) {           /* remove one occurrence of the smallest prime */
            exp--;                    /* for the largest proper divisor            */
            if (exp == 0) continue;
        }
        uint64_t baseMod = _primes[i] % 360ULL;
        for (uint64_t e = 0; e < exp; ++e) {
            acc = (acc * baseMod) % 360ULL;
        }
    }
    return (seq_t)acc;
}
