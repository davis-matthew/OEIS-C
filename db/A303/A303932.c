/*
 * A303932  Number of ways to write 2*n as p + 2^k + 3^m, where p is a prime
 *          with 11 a quadratic residue modulo p, and k and m are nonnegative
 *          integers.
 * https://oeis.org/A303932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A303932:  a(n) = #{(k,m,p) : 2*n = p + 2^k + 3^m,
   p prime with 11 a quadratic residue modulo p}
   -- computed by brute force up to 2*n <= 20000 (n <= 10000) */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/jacobi_symbol.h"

seq_t a(idx_t n)
{
    /* Maximum 2*n to be handled (20000 is enough for n <= 10000). */
    static const size_t LIMIT = 20000;

    /* One‑time data structures, built on the first call. */
    static bool *is_prime   = NULL;
    static bool *prime_ok   = NULL;
    static idx_t *pow2_tbl  = NULL;
    static idx_t *pow3_tbl  = NULL;
    static size_t pow2_cnt = 0, pow3_cnt = 0;

    if (!is_prime) {
        /* ---------- Sieve of Eratosthenes up to LIMIT ---------- */
        is_prime = (bool *)calloc(LIMIT + 1, sizeof(bool));
        if (!is_prime) return 0;
        for (size_t i = 2; i <= LIMIT; ++i) is_prime[i] = true;
        for (size_t i = 2; (idx_t)i * i <= LIMIT; ++i)
            if (is_prime[i])
                for (size_t j = i * i; j <= LIMIT; j += i)
                    is_prime[j] = false;

        /* ---------- Which primes have 11 as a quadratic residue? ---------- */
        prime_ok = (bool *)calloc(LIMIT + 1, sizeof(bool));
        if (!prime_ok) return 0;
        for (idx_t p = 2; p <= LIMIT; ++p) {
            if (!is_prime[p]) continue;
            if (p == 2) {                     /* 11 ≡ 1 (mod 2) is a residue */
                prime_ok[p] = true;
            } else if (jacobi_symbol(11, p) == 1) {
                prime_ok[p] = true;
            }
        }

        /* ---------- Pre‑compute powers of 2 up to LIMIT ---------- */
        idx_t v = 1;
        while (v <= LIMIT) { ++pow2_cnt; v <<= 1; }
        pow2_tbl = (idx_t *)calloc(pow2_cnt, sizeof(idx_t));
        v = 1;
        for (size_t i = 0; i < pow2_cnt; ++i) { pow2_tbl[i] = v; v <<= 1; }

        /* ---------- Pre‑compute powers of 3 up to LIMIT ---------- */
        v = 1;
        while (v <= LIMIT) { ++pow3_cnt; v *= 3; }
        pow3_tbl = (idx_t *)calloc(pow3_cnt, sizeof(idx_t));
        v = 1;
        for (size_t i = 0; i < pow3_cnt; ++i) { pow3_tbl[i] = v; v *= 3; }
    }

    seq_t count = 0;
    idx_t target = 2 * n;

    for (size_t ki = 0; ki < pow2_cnt && pow2_tbl[ki] <= target; ++ki) {
        idx_t pow2k = pow2_tbl[ki];
        for (size_t mi = 0; mi < pow3_cnt && pow2k + pow3_tbl[mi] <= target; ++mi) {
            idx_t sum = pow2k + pow3_tbl[mi];
            idx_t p   = target - sum;
            if (p < 2 || p > LIMIT) continue;
            if (prime_ok[p]) ++count;
        }
    }
    return count;
}
