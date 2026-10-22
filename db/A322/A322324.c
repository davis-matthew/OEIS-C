/*
 * A322324  Square array A(n,k), n >= 1, k >= 0, read by antidiagonals: A(n,k)
 *          = Product_{p|n, p prime} (1 - p^k).
 * https://oeis.org/A322324
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = Product_{p|m} (1 - p^k) where (m,k) are the pair obtained
 *            from the index n by antidiagonal ordering of the square array
 *            A(m,k). The sequence is read first along diagonals of constant m
 *            + k
 * Range:     i8 n <= 30, i16 n <= 70, i32 n <= 158, i64 n <= 415,
 *            i128 n <= 1138, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t
pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1ULL)
            result *= base;
        base *= base;
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t idx)
{
    /*--- 1. Find diagonal s and offset within it -----------------*/
    idx_t s = 0;
    /* (s+1)(s+2)/2 is the number of terms up to and including diagonal s */
    while (((s + 1ULL) * (s + 2ULL)) / 2ULL < idx)
        ++s;

    idx_t prev = (s * (s + 1ULL)) / 2ULL; /* terms before diagonal s */
    idx_t offset = idx - prev;           /* 1‑based index inside diagonal s */
    idx_t m = offset;                    /* row number (>=1) */
    idx_t k = s - offset + 1ULL;         /* column number (>=0) */

    /*--- 2. Compute the product over distinct prime divisors of m  */
    seq_t result = 1;

    /* Special case: m == 1 -> product over empty set = 1 */
    if (m != 1ULL) {
        uint64_t temp = (uint64_t)m;
        for (uint64_t p = 2ULL; p * p <= temp; ++p) {
            if (temp % p == 0ULL) {
                seq_t pk = pow_seq((seq_t)p, k);
                result *= (1 - pk);
                while (temp % p == 0ULL)
                    temp /= p;
            }
        }
        if (temp > 1ULL) { /* remaining prime factor */
            seq_t pk = pow_seq((seq_t)temp, k);
            result *= (1 - pk);
        }
    }

    return result;
}
