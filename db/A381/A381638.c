/*
 * A381638  Numerators of Sum_{i=1..omega(n)-1} p_{i}/p_{i+1}, where omega(n)
 *          = A001221(n) and p_1 < p_2 < ... p_omega(n) are the distinct prime
 *          factors of n; a(1) = 0.
 * https://oeis.org/A381638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of sum_{i=1..omega(n)-1} p_i/p_{i+1} where p_1
 *            < p_2 < ... are the distinct prime factors of n; a(1) = 0
 * Range:     u8 n <= 329, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 9701 (2026-11-01)
 * Helpers:   factor_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"

static void sort_uint64(uint64_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        for (size_t j = i; j > 0 && arr[j-1] > arr[j]; --j) {
            uint64_t tmp = arr[j-1];
            arr[j-1] = arr[j];
            arr[j] = tmp;
        }
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* Factor n into distinct primes. */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    if (cnt <= 1) return 0;             /* n is a prime power */

    /* The factor_u64 routine may return primes unsorted.  Sort them. */
    sort_uint64(primes, cnt);

    /* Compute the sum of consecutive prime ratios as a reduced fraction. */
    seq_t num = 0;
    seq_t den = 1;

    for (size_t i = 0; i + 1 < cnt; ++i) {
        seq_t p     = primes[i];
        seq_t next  = primes[i+1];
        seq_t new_num = num * next + p * den;
        seq_t new_den = den * next;
        seq_t g = gcd(new_num, new_den);
        if (g != 1) {
            new_num /= g;
            new_den /= g;
        }
        num = new_num;
        den = new_den;
    }

    return num;
}
