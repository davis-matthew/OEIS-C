/*
 * A345932  a(n) = A002034(n) / gcd(n, A002034(n)), where A002034(n) gives the
 *          smallest positive integer k such that n divides k!.
 * https://oeis.org/A345932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 297, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 141 terms through n = 9456 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = A002034(n) / gcd( n, A002034(n) )   */
/*  where A002034(n) is the least k with n | k! */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

/* Return the least m such that the exponent of prime p in m! is at least e. */
static idx_t min_m_for_prime_exp(idx_t e, idx_t p)
{
    /* ---- Find an upper bound  ------------------------------------------------- */
    idx_t high = p;
    while (true) {
        idx_t cnt = 0;
        idx_t pow = p;
        while (pow <= high) {
            cnt += high / pow;
            if (pow > high / p) break;
            pow *= p;
        }
        if (cnt >= e) break;
        high = high * 2;
    }

    /* ---- Binary search for minimal m ---------------------------------------- */
    idx_t low = 0;
    while (low + 1 < high) {
        idx_t mid = (low + high) / 2;
        idx_t cnt = 0;
        idx_t pow = p;
        while (pow <= mid) {
            cnt += mid / pow;
            if (pow > mid / p) break;
            pow *= p;
        }
        if (cnt >= e)
            high = mid;
        else
            low = mid;
    }
    return high;
}

/* Compute A002034(n): the smallest k with n | k! */
static idx_t compute_A002034(idx_t n)
{
    if (n <= 1)
        return (idx_t)1;

    idx_t temp = n;
    idx_t result = 1;

    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            idx_t m = min_m_for_prime_exp(e, p);
            if (m > result)
                result = m;
        }
    }
    if (temp > 1) {          /* remaining prime factor */
        idx_t m = min_m_for_prime_exp(1, temp);
        if (m > result)
            result = m;
    }
    return result;
}

/* Public function required by the exercise */
seq_t a(idx_t n)
{
    idx_t k = compute_A002034(n);           /* A002034(n) */
    seq_t k_seq = (seq_t)k;
    seq_t n_seq = (seq_t)n;
    seq_t g = gcd(k_seq, n_seq);
    return k_seq / g;
}
