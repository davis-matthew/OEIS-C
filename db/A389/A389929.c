/*
 * A389929  Smallest prime number obtained by removing or adding a single
 *          digit anywhere in n, or 0 if no such prime exists.
 * https://oeis.org/A389929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest prime obtainable by removing or adding a single
 *            digit in the decimal representation of n, returning 0 if no such
 *            prime exists
 * Range:     u8 n <= 13, u16 n <= 6552, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 92 terms through n = 9763 (2026-11-03)
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

#define MAX_DIG 64  /* enough for indices up to 10^18 and beyond */

seq_t a(idx_t n)
{
    /* Convert n to its decimal digits (most significant first). */
    uint8_t digits[MAX_DIG];
    int dlen = 0;
    idx_t temp = n;

    if (temp == 0) {
        digits[0] = 0;
        dlen = 1;
    } else {
        while (temp > 0 && dlen < MAX_DIG) {
            digits[dlen++] = temp % 10;
            temp /= 10;
        }
        /* reverse to get most‑significant first */
        for (int i = 0; i < dlen / 2; ++i) {
            uint8_t t = digits[i];
            digits[i] = digits[dlen - 1 - i];
            digits[dlen - 1 - i] = t;
        }
    }

    seq_t best = 0;   /* 0 means no prime found yet */

    /* 1. Try removing each digit */
    for (int i = 0; i < dlen; ++i) {
        seq_t val = 0;
        for (int j = 0; j < dlen; ++j) {
            if (j == i) continue;
            val = val * 10 + digits[j];
        }
        if (val != 0 && is_prime_big(val)) {
            if (best == 0 || val < best) best = val;
        }
    }

    /* 2. Try inserting a digit (0–9) at every position (0…dlen) */
    for (int pos = 0; pos <= dlen; ++pos) {
        for (int d = 0; d <= 9; ++d) {
            seq_t val = 0;
            for (int j = 0; j < pos; ++j) val = val * 10 + digits[j];
            val = val * 10 + d;
            for (int j = pos; j < dlen; ++j) val = val * 10 + digits[j];
            if (val != 0 && is_prime_big(val)) {
                if (best == 0 || val < best) best = val;
            }
        }
    }

    return best;
}
