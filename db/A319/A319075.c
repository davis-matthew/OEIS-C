/*
 * A319075  Square array T(n,k) read by antidiagonal upwards in which row n
 *          lists the n-th powers of primes, hence column k lists the powers
 *          of the k-th prime, n >= 0, k >= 1.
 * https://oeis.org/A319075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = prime(k)^(s-r) where s is the largest integer with
 *            s*(s+1)/2 <= n, k = (n - s*(s+1)/2) + 1 and exp = s - (n -
 *            s*(s+1)/2)
 * Range:     u8 n <= 22, u16 n <= 46, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   get_prime, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/seq_pow.h"

/* Find the largest s such that s*(s+1)/2 <= n */
static idx_t diag_index(idx_t n)
{
    idx_t lo = 0, hi = 1;
    while ((__int128)hi * (hi + 1) / 2 <= n) hi <<= 1;
    while (lo + 1 < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            lo = mid;
        else
            hi = mid;
    }
    return lo;
}

/* Sequence A319075 */
seq_t a(idx_t n)
{
    idx_t s      = diag_index(n);                     /* diagonal number   */
    idx_t before = (s * (s + 1)) / 2;                 /* count before diag */
    idx_t r      = n - before;                        /* offset inside diag*/
    idx_t k      = r + 1;                              /* column (prime index) */
    idx_t exp    = s - r;                              /* row (exponent)      */

    uint32_t prime_val = get_prime(k);                /* k-th prime (1‑based)*/
    seq_t base = (seq_t)prime_val;
    return seq_pow(base, exp);                         /* prime^exp          */
}
