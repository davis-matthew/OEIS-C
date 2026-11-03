/*
 * A337834  If k is the n-th number ending in 1,3,7, or 9, a(n) is the least
 *          prime > k ending in k.
 * https://oeis.org/A337834
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 403, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9875 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: For the n‑th integer k ending in 1,3,7, or 9, find the
   least prime p > k that ends with the digits of k.  The prime p is
   the smallest number in the arithmetic progression k, k+10^d, k+2·10^d,
   … that is prime, where d = digit_len(k). */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
#include "../utils/is_prime_u64.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    if (n == 0) return 0;           /* undefined for offset 0, return 0 */

    /* Find the n‑th integer ending in 1,3,7,9 */
    idx_t q = (n - 1) / 4;           /* block number */
    idx_t r = (n - 1) % 4;           /* position within the block */
    uint8_t digit = (r == 0) ? 1
                 : (r == 1) ? 3
                 : (r == 2) ? 7
                 : 9;

    /* Compute k = 10·q + digit, cast to seq_t */
    seq_t k = (seq_t)(10 * (seq_t)q + (seq_t)digit);

    /* Determine number of decimal digits of k */
    int d = num_digits(k);

    /* Modulus for the arithmetic progression: 10^d */
    seq_t mod = pow10(d);

    /* First candidate > k in the progression */
    seq_t cand = k + mod;

    /* Search for the least prime in this progression */
    while (!is_prime_big(cand)) {
        cand += mod;
    }

    return cand;
}
