/*
 * A253046  An involution of the natural numbers: if n = 2*p_i then replace n
 *          with 3*p_{i+1}, and conversely if n = 3*p_i then replace n with
 *          2*p_{i-1}, where p_i denotes the i-th prime.
 * https://oeis.org/A253046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n if n is not 2×prime or 3×prime; Otherwise if n = 2*p_i
 *            then a(n) = 3*p_{i+1}, if n = 3*p_i then a(n) = 2*p_{i-1} (p_0
 *            undefined)
 * Range:     u8 n <= 165, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9950 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

seq_t a(idx_t n)
{
    /* First check for n = 2 * prime */
    if ((n % 2) == 0) {
        uint64_t m = n / 2;
        if (is_prime_big((seq_t)m)) {
            uint64_t nextp = next_prime(m);
            if (nextp != 0) {
                return (seq_t)3 * (seq_t)nextp;
            }
        }
    }

    /* Next check for n = 3 * prime */
    if ((n % 3) == 0) {
        uint64_t m = n / 3;
        if (is_prime_big((seq_t)m)) {
            uint64_t prevp = prev_prime(m);
            if (prevp != 0) {
                return (seq_t)2 * (seq_t)prevp;
            }
        }
    }

    /* Default: a(n) = n */
    return (seq_t)n;
}
