/*
 * A278247  a(n) = least number with the same prime signature as n + (n+1)^2.
 * https://oeis.org/A278247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least number with the same prime signature as n + (n+1)^2
 *            Computed as a(n) = A046523(A028387(n)) where A028387(n) = n +
 *            (n+1)^2 A046523(m) = minimal integer having the same multiset of
 *            exponents as m
 * Range:     u8 n <= 14, u16 n <= 269, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Compute m = n + (n+1)^2 into seq_t first to avoid overflow */
    seq_t np1 = (seq_t)n + 1;
    seq_t m    = (seq_t)n + (np1 * np1);

    /* For the range of interest m fits into uint64_t, cast safely */
    uint64_t m64 = (uint64_t)m;

    /* Factor m; we store primes and exponents into small arrays.
       For m <= ~10^8 the number of prime factors never exceeds 20. */
    static const uint64_t prime_buf[25] = {0};
    static const uint64_t exp_buf[25]   = {0};
    uint64_t pr[25];
    uint64_t ex[25];
    size_t  cnt = 0;
    factor_u64(m64, pr, ex, &cnt);

    /* If m == 1, the signature is empty and the minimal number is 1. */
    if (cnt == 0)
        return (seq_t)1;

    /* Gather the exponents into an array of integers and sort them
       in decreasing order (largest exponent first). */
    int exp_list[25];
    for (size_t i = 0; i < cnt; ++i)
        exp_list[i] = (int)ex[i];

    /* Simple sort descending: insertion sort is adequate for very small n. */
    for (size_t i = 1; i < cnt; ++i) {
        int key = exp_list[i];
        size_t j = i;
        while (j > 0 && exp_list[j - 1] < key) {
            exp_list[j] = exp_list[j - 1];
            --j;
        }
        exp_list[j] = key;
    }

    /* List of the first few primes, enough for any reasonable exponent count. */
    static const seq_t primes_first[25] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
        73ULL, 79ULL, 83ULL, 89ULL, 97ULL
    };

    /* Build the minimal number by assigning the largest exponent to the
       smallest prime, second largest to the second smallest, etc. */
    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        seq_t base = primes_first[i];
        int e = exp_list[i];
        /* Multiply by base^e. Since exponents are tiny, naive loop is fine. */
        for (int k = 0; k < e; ++k)
            result *= base;
    }

    return result;
}
