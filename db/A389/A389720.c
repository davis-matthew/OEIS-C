/*
 * A389720  a(n) is the largest prime number obtainable by inserting a single
 *          digit anywhere in n (including at the beginning or end), or -1 if
 *          no such prime is obtainable.
 * https://oeis.org/A389720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base
 * Formula:   a(n) = largest prime obtainable by inserting a single decimal
 *            digit anywhere in n (including at the start or end), or -1 if
 *            none
 * Range:     i8 n <= 10, i16 n <= 1000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9682 (2026-11-03)
 * Helpers:   is_prime_u64, num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n) {
    /* Convert the index to a seq_t value */
    seq_t orig = (seq_t)n;

    /* The best prime found so far, -1 if none */
    seq_t best = (seq_t)-1;

    /* Number of decimal digits of the original number */
    int d = num_digits(orig);
    if (d < 0) return (seq_t)-1;  /* safety: should not happen */

    /* For each insertion position */
    for (idx_t pos = 0; pos <= (idx_t)d; ++pos) {
        /* 10^(d-pos) */
        seq_t pow_low = pow10(d - pos);
        seq_t high = orig / pow_low;
        seq_t low  = orig % pow_low;

        /* Try inserting each decimal digit */
        for (int dig = 0; dig <= 9; ++dig) {
            seq_t cand = high * pow_low * 10 + (seq_t)dig * pow_low + low;
            if (cand < 2) continue;             /* ignore numbers < 2 */
            if (is_prime_big(cand) && cand > best) {
                best = cand;
            }
        }
    }

    return best;
}
