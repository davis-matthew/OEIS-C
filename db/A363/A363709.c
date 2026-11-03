/*
 * A363709  For n >= 0, a(n) is the least integer i >= 0 such that n + (n + 1)
 *          + ... + (n + i) is a prime number or a(n) = -1 if no such i
 *          exists.
 * https://oeis.org/A363709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least i ≥ 0 such that (i+1)*(2n+i)/2 is prime; a(0) = 2
 *            (since 0+1+2 = 3 is the first prime), for n ≥ 1 a(n) is 0 if n
 *            is prime, 1 if 2n+1 is prime, otherwise -1
 * Range:     i8 n <= 64, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
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
    if (n == 0u) {
        /* 0 + 1 + 2 = 3 is prime → minimal i = 2 */
        return (seq_t)2;
    }

    /* i = 0: sum = n */
    if (is_prime_big((seq_t)n))
        return (seq_t)0;

    /* i = 1: sum = n + (n+1) = 2*n + 1 */
    seq_t sum2 = (seq_t)2 * (seq_t)n + (seq_t)1;
    if (is_prime_big(sum2))
        return (seq_t)1;

    /* No i ≤ 1 produces a prime, so return -1 for n ≥ 1 */
    return (seq_t)-1;
}
