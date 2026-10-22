/*
 * A381032  The radix prime of the least significant digit > 1 in the
 *          primorial base expansion of n, or 1 if there is no such digit.
 * https://oeis.org/A381032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = radix prime of the least significant digit > 1 in the
 *            primorial base expansion of n, or 1 if there is no such digit.
 *            The radix prime is the (position+1)-th prime where the position
 *            is counted from 1 for the least significant digit
 * Range:     u8 n <= 9976, u16 n <= 9976, u32 n <= 9976, u64 n <= 9976,
 *            u128 n <= 9976, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

/* A small but sufficient list of primes.  The primorial base for n
   never needs more than about 20 primes for n up to 2^64-1, so this
   buffer is ample. */
static const uint64_t _primes[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
    73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL,
    113ULL, 127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL,
    163ULL, 167ULL, 173ULL, 179ULL, 181ULL, 191ULL, 193ULL, 197ULL,
    199ULL, 211ULL, 223ULL, 227ULL, 229ULL, 233ULL, 239ULL, 241ULL,
    251ULL, 257ULL, 263ULL, 269ULL, 271ULL, 277ULL, 281ULL, 283ULL,
    293ULL, 307ULL, 311ULL
};
static const size_t _primes_cnt = sizeof _primes / sizeof _primes[0];

seq_t a(idx_t n)
{
    /* For n = 0 or 1 the representation contains no digit > 1. */
    if (n < 2) return (seq_t)1;

    /* Build primorial weights up to the largest weight ≤ n. */
    idx_t weights[_primes_cnt];
    size_t last_pos = (size_t)-1;        /* index of the last valid weight */
    idx_t prod = 1;

    for (size_t i = 0; i < _primes_cnt; ++i) {
        /* Avoid overflow: break if the next multiplication would exceed n. */
        if (prod > n / _primes[i]) break;
        prod *= _primes[i];
        weights[i] = prod;
        if (prod <= n) last_pos = i;
    }

    /* Compute the primorial base digits from most significant to least. */
    idx_t rem = n;
    idx_t digits[_primes_cnt];  /* only entries up to last_pos are used */

    for (size_t idx = last_pos; idx != (size_t)-1; --idx) {
        idx_t w = weights[idx];
        idx_t d = rem / w;
        rem -= d * w;
        digits[idx] = d;
        if (idx == 0) break;
    }

    /* Find the least significant digit that is > 1. */
    for (size_t pos = 0; pos <= last_pos; ++pos) {
        if (digits[pos] > (idx_t)1) {
            /* The required prime is the next prime after the base
               prime for this position. */
            size_t prime_index = pos + 1;          /* 0‑based index */
            if (prime_index < _primes_cnt) {
                return (seq_t)_primes[prime_index];
            }
            /* In practice this case never occurs with the supplied
               prime table. */
            return (seq_t)1;
        }
    }

    return (seq_t)1;
}
