/*
 * A269302  Normalization coefficients for quantum Pascal's pyramid,
 *          denominators of: T(n,k,m) = ((n - m)! m!)/(2^n (n - k)! k!).
 * https://oeis.org/A269302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   sqsum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = denominator of ((n - m)! * m!)/( 2^n * (n - k)! * k! ),
    where n,k,m are determined from the block index:
    let S(n) = n(n+1)(2n+1)/6.  Find the unique n with
          S(n) <= n_var < S(n+1).
    Then  base = n + 1,
          d    = n_var - S(n),
          k    = d / base,
          m    = d % base.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sqsum.h"

#define MAX_PRIMES 20000
static unsigned int primes_arr[MAX_PRIMES];
static int primes_cnt = 0;

/* Compute primes up to n (inclusive) ---------------------------------------- */
static void ensure_primes(unsigned int n) {
    if (n < 2) {
        /* no primes needed for the main loop; 2 is handled separately */
        return;
    }
    if (primes_cnt && primes_arr[primes_cnt - 1] >= n) return;

    unsigned int start = (primes_cnt ? primes_arr[primes_cnt - 1] + 1 : 2);
    for (unsigned int x = start; x <= n; ++x) {
        bool is_pr = true;
        for (int i = 0; i < primes_cnt && primes_arr[i] * primes_arr[i] <= x; ++i) {
            if (x % primes_arr[i] == 0) {
                is_pr = false;
                break;
            }
        }
        if (is_pr) {
            if (primes_cnt < MAX_PRIMES) primes_arr[primes_cnt++] = x;
        }
    }
}

/* Exponent of prime p in n! ----------------------------------------------- */
static int legendre(unsigned int n, unsigned int p) {
    int e = 0;
    while (n) {
        n /= p;
        e += n;
    }
    return e;
}

/* Fast exponentiation for seq_t ------------------------------------------------ */
static inline seq_t pow_seq(seq_t base, int exp) {
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* -------------------------------------------------------------------------- */
seq_t a(idx_t idx) {
    /* find block number n such that S(n) <= idx < S(n+1) */
    unsigned int low = 0, high = 1;
    seq_t target = (seq_t)idx;
    while (sqsum(high) <= target && high < UINT32_MAX / 2U) high <<= 1;

    while (low < high) {
        unsigned int mid = low + (high - low) / 2;
        if (sqsum(mid) <= target)
            low = mid + 1;
        else
            high = mid;
    }
    unsigned int n = low - 1;           /* block index */

    seq_t base_squares = sqsum(n);
    seq_t d = target - base_squares;    /* 0 <= d < (n+1)^2 */
    unsigned int base = n + 1;
    unsigned int k = (unsigned int)(d / base);
    unsigned int m = (unsigned int)(d % base);

    /* Prepare primes up to n for remaining primes (>2) */
    ensure_primes(n);

    /* Compute exponent of 2 in the denominator (including 2^n) */
    int e2 = legendre(n - k, 2) + legendre(k, 2) + (int)n
            - (legendre(n - m, 2) + legendre(m, 2));
    if (e2 < 0) e2 = 0;                 /* safety */

    seq_t denom = pow_seq((seq_t)2, e2);

    /* Process other primes (>2) */
    for (int i = 0; i < primes_cnt; ++i) {
        unsigned int p = primes_arr[i];
        if (p == 2) continue;           /* already handled */

        int e_num = legendre(n - m, p) + legendre(m, p);
        int e_den = legendre(n - k, p) + legendre(k, p);

        int common = e_num < e_den ? e_num : e_den;
        int e_denom = e_den - common;
        if (e_denom > 0)
            denom *= pow_seq((seq_t)p, e_denom);
    }

    return denom;
}
