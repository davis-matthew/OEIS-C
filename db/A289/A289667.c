/*
 * A289667  Concatenate prime factorization written in base 3, convert back to
 *          decimal.
 * https://oeis.org/A289667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of (prime in base 3), and if the exponent >
 *            1, concatenation of (exponent in base 3), interpreted as a
 *            base‑3 number
 * Range:     u8 n <= 59, u16 n <= 3431, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   append_base3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A289667 – Concatenate prime factorization written in base 3, convert back to decimal.
 *  For each prime factor p^e of n (n ≥ 2),
 *      write p in base 3.
 *      if e > 1, write e in base 3 immediately after p.
 *  The resulting string of digits is interpreted as a base‑3 number; that decimal value
 *  is a(n).  For n = 1, a(1) = 1.
 *  Example:  n = 12 = 2^2 * 3^1
 *             2 → "2" , e=2 → "2"   => "22"
 *             3 → "10"  (e=1, so no exponent)
 *             concatenation = "2210" (base 3) = 75 in decimal.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/append_base3.h"

/* Ensure this matches the compiler's typedefs for seq_t and idx_t. */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;   /* Special case for n = 1. */

    /* Factor n via trial division (fast enough for n ≤ 10000). */
    idx_t temp = n;
    idx_t factors[32];
    int exps[32];
    size_t count = 0;

    for (idx_t p = 2; p <= temp / p; ++p) {
        if (temp % p == 0) {
            int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            factors[count] = p;
            exps[count] = e;
            ++count;
        }
    }
    if (temp > 1) {                 /* Remaining prime factor. */
        factors[count] = temp;
        exps[count] = 1;
        ++count;
    }

    /* Build the concatenated base‑3 string. */
    unsigned char digits[512];
    size_t digits_len = 0;
    for (size_t i = 0; i < count; ++i) {
        append_base3(factors[i], digits, &digits_len);
        if (exps[i] > 1) {
            append_base3((idx_t)exps[i], digits, &digits_len);
        }
    }

    /* Convert the digit string from base 3 to decimal in seq_t. */
    seq_t result = 0;
    for (size_t i = 0; i < digits_len; ++i) {
        result = result * 3 + digits[i];
    }
    return result;
}
