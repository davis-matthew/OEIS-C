/*
 * A382277  a(n) is the least composite number obtained by inserting a
 *          nonempty string of 0's inside n.
 * https://oeis.org/A382277
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9659 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the least composite number obtained by inserting a nonempty
 * string of 0's inside the decimal representation of the index n.
 * For each position between two consecutive digits of n, we try inserting
 * 1, 2, 3, ... zeros until we get a composite number, then keep the
 * smallest such composite among all positions.
 * n is assumed to be >= 10; behaviour for smaller n is undefined.
 */

#include <stdbool.h>
#include <stdint.h>
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
    /* If n is less than 10 we are outside the defined region; return 0.
     * The sequence is only needed for n >= 10. */
    if (n < 10) return 0;

    /* Extract decimal digits of n into an array, most‑significant first. */
    idx_t temp = n;
    idx_t digits[20];  /* Enough for 64‑bit integers (~20 digits). */
    idx_t k = 0;
    while (temp > 0) {
        digits[k++] = temp % 10;
        temp /= 10;
    }
    /* Reverse to get most‑significant first. */
    for (idx_t i = 0; i < k / 2; ++i) {
        idx_t tmp = digits[i];
        digits[i] = digits[k - 1 - i];
        digits[k - 1 - i] = tmp;
    }

    seq_t best = 0;  /* 0 indicates no candidate yet. */

    /* Iterate over all positions between digits. */
    for (idx_t pos = 0; pos + 1 < k; ++pos) {
        /* Try inserting t zeros, starting from 1. */
        for (idx_t t = 1; ; ++t) {
            seq_t val = 0;
            for (idx_t j = 0; j < k; ++j) {
                val = val * 10 + digits[j];
                if (j == pos) {
                    /* Insert t zeros after this digit. */
                    val *= pow10(t);
                }
            }
            /* Check if val is composite. */
            if (!is_prime_big(val)) {
                if (best == 0 || val < best) best = val;
                break;  /* Found the smallest composite for this position. */
            }
            /* For safety, stop if t becomes too large (should not happen). */
            if (t > 10) break;
        }
    }

    return best;
}
