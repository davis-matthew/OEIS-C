/*
 * A289732  Primes of the form k!6-18, where k!6 is the sextuple factorial
 *          number (A085158).
 * https://oeis.org/A289732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime numbers p such that p + 18 = k!6 where k!6 =
 *            product of all integers ≤ k that are congruent to k modulo 6
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Validated: standard, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
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

seq_t a(idx_t n) {
    /* Product of numbers ≡ r (mod 6) seen so far, for each r = 0 … 5. */
    seq_t prod[6] = {1, 1, 1, 1, 1, 1};

    seq_t candidate;
    size_t seen = 0;
    idx_t k = 0;

    while (seen < n) {
        ++k;
        int r = k % 6;
        seq_t tmp = prod[r] * k;

        /* Detect integer overflow (if seq_t is an unsigned built‑in type). */
        if (tmp < prod[r])
            break;                    /* no further values fit in seq_t */

        prod[r] = tmp;

        if (tmp > (seq_t)18) {        /* avoid underflow when subtracting 18 */
            candidate = tmp - (seq_t)18;
            if (is_prime_big(candidate)) {
                ++seen;
                if (seen == n)
                    return candidate;
            }
        }
    }
    return (seq_t)0;                    /* should not happen for valid inputs */
}
