/*
 * A375968  a(n) is the least number with the same digit average as n.
 * https://oeis.org/A375968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) equals the smallest decimal integer whose digit * average
 *            (sum of digits ÷ number of digits) equals that of n
 * Range:     u8 n <= 298, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"   /* gcd(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    /* Special case: n = 0.  Its digit average is 0, and the least
     * number with average 0 is 0 itself. */
    if (n == 0) {
        return 0;
    }

    /* ---- Compute digit sum and digit count of n ---- */
    idx_t tmp = n;
    seq_t sum = 0;      /* 0 ≤ sum ≤ 9⋅digits ≤ 180, fits in seq_t */
    int    d   = 0;     /* digit count (1–20 for 64‑bit numbers) */
    do {
        sum += tmp % 10;
        tmp /= 10;
        ++d;
    } while (tmp);

    /* If the digit sum is 0 (only possible for n = 0) the above
     * special case handles it. */
    if (sum == 0) {
        return 0;
    }

    /* ---- Reduce the average fraction ---- */
    seq_t g  = gcd(sum, (seq_t)d);   /* 1 ≤ g ≤ min(sum,d) */
    seq_t p  = sum / g;              /* numerator of reduced fraction */
    seq_t q  = (seq_t)d / g;         /* denominator of reduced fraction */

    /* The minimal length L that can realise this average is L = q.
     * The required digit sum for that length is S = p. */
    const uint64_t L = (uint64_t)q;
    seq_t          S = p;

    /* ---- Construct the minimal L‑digit number with digit sum S ---- */
    seq_t result   = 0;
    seq_t sum_rem  = S;

    for (uint64_t pos = 0; pos < L; ++pos) {
        uint64_t rem_len          = L - pos;
        seq_t    min_digit        = (pos == 0 && rem_len > 1) ? 1 : 0;
        seq_t    max_possible_rem = 9 * (seq_t)(rem_len - 1);

        /* The smallest digit that keeps the rest achievable. */
        seq_t digit = min_digit;
        if (sum_rem > max_possible_rem + min_digit) {
            digit = sum_rem - max_possible_rem;
            if (digit < min_digit) digit = min_digit;
        }
        if (digit > 9) digit = 9;   /* safety, should never happen */

        sum_rem -= digit;
        result = result * 10 + digit;
    }

    return result;
}
