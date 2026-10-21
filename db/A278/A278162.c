/*
 * A278162  Least number with the prime signature of n^2 + 1.
 * https://oeis.org/A278162
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523((n^2)+1) – minimal integer with the same *
 *            multiset of exponents (prime signature) as n²+1
 * Range:     u8 n <= 15, u16 n <= 553, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_pow.h"

static const seq_t SMALL_PRIMES[12] = {
    (seq_t)2, (seq_t)3, (seq_t)5, (seq_t)7,
    (seq_t)11, (seq_t)13, (seq_t)17, (seq_t)19,
    (seq_t)23, (seq_t)29, (seq_t)31, (seq_t)37
};

seq_t a(idx_t n)
{
    /* m = n² + 1 */
    seq_t m = (seq_t)n * (seq_t)n + (seq_t)1;

    /* For m == 1 there are no prime factors */
    if (m == (seq_t)1) return (seq_t)1;

    int exps[32];
    size_t ecnt = 0;

    seq_t tmp = m;
    /* Trial division up to √tmp */
    for (idx_t p = 2; (seq_t)p * (seq_t)p <= tmp; ++p) {
        if (tmp % (seq_t)p == (seq_t)0) {
            int cnt = 0;
            while (tmp % (seq_t)p == (seq_t)0) {
                tmp /= (seq_t)p;
                ++cnt;
            }
            exps[ecnt++] = cnt;
        }
    }
    /* Remaining prime factor, if any */
    if (tmp > (seq_t)1) {
        exps[ecnt++] = 1;
    }

    /* Sort exponents in descending order (to assign the largest to the
     * smallest primes for minimal result) */
    for (size_t i = 1; i < ecnt; ++i) {
        int val = exps[i];
        size_t j = i;
        while (j > 0 && exps[j - 1] < val) {
            exps[j] = exps[j - 1];
            --j;
        }
        exps[j] = val;
    }

    /* Build the minimal number from the sorted exponents */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < ecnt; ++i) {
        result *= seq_pow(SMALL_PRIMES[i], (idx_t)exps[i]);
    }
    return result;
}
