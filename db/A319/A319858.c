/*
 * A319858  a(n) is the number of values of m in the interval [2,10] such that
 *          the base-m expansion of n, interpreted as a base-10 number, yields
 *          a prime.
 * https://oeis.org/A319858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Validated: partial, pass, 100 terms through n = 100 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count the number of bases m ∈ [2,10] for which the base‑m expansion of n,
 * interpreted as a decimal integer, is prime. */
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

static bool expansion_is_prime(idx_t n, unsigned base)
{
    /* Convert n to base 'base' (least‑significant digit first). */
    unsigned char digits[80];
    idx_t tmp = n;
    unsigned dcnt = 0;

    if (tmp == 0) {
        digits[dcnt++] = 0;
    } else {
        while (tmp > 0) {
            digits[dcnt++] = (unsigned char)(tmp % base);
            tmp /= base;
        }
    }

    /* Build the decimal integer from the most‑significant digit first. */
    uint64_t val64 = 0;
    for (int i = (int)dcnt - 1; i >= 0; --i) {
        val64 = val64 * 10 + digits[i];
    }

    /* Cast to seq_t (unsigned) and test primality. */
    seq_t val = (seq_t)val64;
    return is_prime_big(val);
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (unsigned base = 2; base <= 10; ++base) {
        if (expansion_is_prime(n, base)) {
            count++;
        }
    }
    return count;
}
