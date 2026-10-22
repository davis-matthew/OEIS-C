/*
 * A381959  Denominator of the sum of the reciprocals of the indices of
 *          distinct prime factors of n.
 * https://oeis.org/A381959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{p|n, p prime} 1/index(p) // OEIS
 *            A381959
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   _smallest_prime_factor, build_primes_upto, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd.h"
#include "../utils/build_primes_upto.h"
#include "../utils/_smallest_prime_factor.h"

seq_t a(idx_t n)
{
    if (n <= 1)  // a(1) = 1 (empty sum)
        return (seq_t)1;

    /* static prime table up to current maximum n */
    static uint64_t *prime_tbl = NULL;
    static size_t prime_cnt = 0;
    static idx_t prime_lim  = 0;

    if (n > prime_lim) {
        free(prime_tbl);
        build_primes_upto(n, &prime_tbl, &prime_cnt);
        prime_lim = n;
    }

    uint64_t temp = (uint64_t)n;
    uint64_t prime_idx[64];      /* enough for 64 distinct primes (n very small) */
    size_t idx_cnt = 0;

    /* factor n extracting distinct primes and collecting their indices */
    while (temp > 1) {
        uint64_t p = _smallest_prime_factor(temp);
        if (p == 0) break;          /* safety: should not happen */
        /* skip duplicate prime factors */
        if (idx_cnt == 0 || p != prime_tbl[prime_idx[idx_cnt-1]-1]) {
            /* binary search for the index of p in prime_tbl */
            idx_t lo = 0, hi = (idx_t)prime_cnt;
            while (lo < hi) {
                idx_t mid = (lo + hi) >> 1;
                if (prime_tbl[mid] < p)
                    lo = mid + 1;
                else
                    hi = mid;
            }
            /* lo is position of p (0‑based) */
            prime_idx[idx_cnt++] = (idx_t)(lo + 1);   /* index is 1‑based */
        }
        /* remove all occurrences of p */
        while (temp % p == 0)
            temp /= p;
    }

    /* compute LCM of the collected indices */
    seq_t L = 1;
    for (size_t i = 0; i < idx_cnt; ++i) {
        seq_t idx = (seq_t)prime_idx[i];
        seq_t g = gcd(L, idx);
        L = (L / g) * idx;
    }

    /* sum numerator over common denominator L */
    seq_t num = 0;
    for (size_t i = 0; i < idx_cnt; ++i) {
        seq_t idx = (seq_t)prime_idx[i];
        num += L / idx;
    }

    seq_t g = gcd(L, num);
    return L / g;   /* denominator in lowest terms */
}
