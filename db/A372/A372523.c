/*
 * A372523  Triangle read by rows: T(n, k) is equal to n/k if k | n, else to
 *          the concatenation of A003988(n, k) = floor(n/k) and A051127(k, n)
 *          = n mod k.
 * https://oeis.org/A372523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, look, tabl
 * Formula:   a(n) = flattening of the triangle T(n,k) where T(n,k) = n/k if k
 *            | n, otherwise the concatenation of floor(n/k) and n mod k
 * Range:     u8 n <= 913, u16 n <= 11325, u32 n <= 11325, u64 n <= 11325,
 *            u128 n <= 11325, big any n
 * Validated: standard, pass, 94 terms through n = 8543 (2026-11-01)
 * Helpers:   num_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utility functions from the provided library */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/num_digits.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset is 1, so a(0) is undefined; return 0 as harmless value */
        return (seq_t)0;
    }

    /* Find the row r such that T(r-1) < n ≤ T(r), where
       T(r) = r*(r+1)/2.  Since n is at most a few thousand in typical tests,
       a linear scan is fast enough and avoids overflow pitfalls. */
    idx_t r = 1;
    idx_t cumulative = 1;          /* T(1) */
    while (cumulative < n) {
        r++;
        cumulative += r;           /* add T(r) = r */
    }

    /* Column position c: 1 ≤ c ≤ r. */
    idx_t prev_cum = cumulative - r;   /* T(r-1) = cumulative - r */
    idx_t c = n - prev_cum;             /* 1‑based column index */

    /* Compute floor division and remainder (both fit in idx_t). */
    idx_t q = r / c;
    idx_t rem = r % c;

    /* If divisible, the value is simply q. */
    if (rem == 0) {
        return (seq_t)q;
    }

    /* Otherwise concatenate q and rem:
       T = q * 10^(digits(rem)) + rem. */
    idx_t digits_rem = num_digits((seq_t)rem); /* number of decimal digits of rem */
    seq_t pow = pow10(digits_rem);            /* 10^digits_rem as seq_t */
    seq_t result = (seq_t)q * pow + (seq_t)rem;

    return result;
}
