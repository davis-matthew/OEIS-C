/*
 * A371240  The sum of divisors of the zeroless numbers in base 3 (A032924).
 * https://oeis.org/A371240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sigma(A032924(n))
 * Range:     u8 n <= 18, u16 n <= 1034, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A032924(n) is the nth base‑3 number with digits only 1 or 2 */
/* For n ≥ 1.  The function returns the sum of positive divisors of that number. */
#include <stdbool.h>

static inline idx_t ceil_log2_u64(idx_t v)
{
    /* return smallest k such that 2^k >= v, v >= 1 */
    idx_t k = 0;
    idx_t pow = 1;
    while (pow < v) {
        pow <<= 1;
        ++k;
    }
    return k;
}

static inline seq_t sigma_uint64(uint64_t m)
{
    seq_t sum = 1;
    uint64_t temp = m;

    /* factor 2 separately */
    if ((temp & 1) == 0) {
        uint64_t cnt = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;
            ++cnt;
        }
        seq_t power = 1;
        for (uint64_t i = 0; i < cnt + 1; ++i)
            power *= (seq_t)2;
        seq_t term = (power - 1) / (seq_t)(2 - 1); /* 2-1 == 1 */
        sum *= term;
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            uint64_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            seq_t power = 1;
            for (uint64_t i = 0; i < cnt + 1; ++i)
                power *= (seq_t)p;
            seq_t term = (power - 1) / (seq_t)(p - 1);
            sum *= term;
        }
    }

    if (temp > 1) {            /* remaining prime factor */
        seq_t term = (seq_t)(temp + 1);  /* sigma(p) = 1 + p */
        sum *= term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* Determine block length k (number of base‑3 digits). */
    idx_t k = ceil_log2_u64(n + 2) - 1;      /* k ≥ 1 for all n ≥ 1 */

    /* Compute the offset within the k‑digit block. */
    idx_t start = ((idx_t)1) << k;          /* 2^k  */
    idx_t j = n - (start - 1);              /* 0 ≤ j < 2^k */

    /* Build the zeroless number from the binary representation of j. */
    seq_t z = 0;
    for (idx_t pos = k; pos > 0; --pos) {
        idx_t bit = (j >> (pos - 1)) & 1ul;
        seq_t digit = bit ? (seq_t)2 : (seq_t)1;
        z = z * 3 + digit;
    }

    /* Compute sigma(z).  z fits in 64 bits for n up to about 10000. */
    uint64_t m = (uint64_t)z;
    return sigma_uint64(m);
}
