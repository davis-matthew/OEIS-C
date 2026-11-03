/*
 * A375548  a(n) is the least prime number of the form n mod (10^k) for some k
 *          > 0, or 0 if no such prime number exists.
 * https://oeis.org/A375548
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least prime p such that p = n mod 10^k for some k>0; 0 if
 *            none
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
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

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* n = 0 or 1 gives no prime suffix */

    seq_t min_prime = 0;          /* 0 means no prime found yet */
    idx_t power = 10;             /* 10^1 */

    /* Check suffixes corresponding to 10, 100, 1000, ... up to power <= n */
    while (power <= n) {
        idx_t r = n % power;
        if (r > 1 && is_prime_big((seq_t)r)) {
            if (min_prime == 0 || (seq_t)r < min_prime)
                min_prime = (seq_t)r;
        }
        /* Avoid overflow of power * 10; stop if next would exceed n */
        if (power > n / 10) break;
        power *= 10;
    }

    /* Also consider the full number itself (k > number of digits) */
    if (n > 1 && is_prime_big((seq_t)n)) {
        if (min_prime == 0 || (seq_t)n < min_prime)
            min_prime = (seq_t)n;
    }

    return min_prime;
}
