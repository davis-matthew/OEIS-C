/*
 * A276154  a(n) = Shift primorial base representation (A049345) of n left by
 *          one digit (append one zero to the right, then convert back to
 *          decimal).
 * https://oeis.org/A276154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k>=0} d_k * P_{k+1} where n = sum_{k>=0} d_k * P_k,
 *            P_k = product of the first k primes (P_0 = 1), and digit d_k
 *            satisfies 0 <= d_k < prime_{k+1}. This is equivalent to shifting
 *            the primorial‑base representation of n left by one digit
 *            (appending a zero to the right)
 * Range:     u8 n <= 5, u16 n <= 645, u32 n <= 9462, u64 n <= 9462,
 *            u128 n <= 9462, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* first 20 primes – more than enough for n up to at least 10 000 */
static const uint64_t _A276154_PRIMES[] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
    31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL
};
#define _A276154_PRIME_COUNT (sizeof(_A276154_PRIMES)/sizeof(_A276154_PRIMES[0]))

seq_t a(idx_t n)
{
    /* base case */
    if (n == (idx_t)0)
        return (seq_t)0;

    /* primorial values, computed once */
    static seq_t primorial[_A276154_PRIME_COUNT + 1];
    static int primorial_inited = 0;
    if (!primorial_inited) {
        primorial[0] = (seq_t)1;
        for (size_t i = 0; i < _A276154_PRIME_COUNT; ++i) {
            primorial[i + 1] = primorial[i] * (seq_t)_A276154_PRIMES[i];
        }
        primorial_inited = 1;
    }

    /* find largest k with primorial[k] <= n */
    size_t k = 0;
    for (size_t i = _A276154_PRIME_COUNT; i > 0; --i) {
        if (primorial[i] <= (seq_t)n) {
            k = i;
            break;
        }
    }

    /* extract primorial‑base digits of n */
    seq_t digits[_A276154_PRIME_COUNT + 1];
    idx_t rem = n;
    for (int j = (int)k; j >= 1; --j) {
        seq_t div = (seq_t)(rem / (idx_t)primorial[j]);
        digits[j] = div;
        rem -= (idx_t)(div * primorial[j]);
    }
    digits[0] = (seq_t)rem;    /* least significant digit */

    /* compute the shifted value */
    seq_t res = (seq_t)0;
    for (size_t j = 0; j <= k; ++j) {
        res += digits[j] * primorial[j + 1];
    }
    return res;
}
