/*
 * A249443  Numbers with digits in nondecreasing order and digital sum not
 *          larger than the product of the digits.
 * https://oeis.org/A249443
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   numbers with digits in nondecreasing order and digital sum <=
 *            product of digits
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Computes the n-th term of OEIS A249443.
   n >= 1, a(1) = 0, subsequent terms are found by testing each integer for
   nondecreasing digits and the condition sum(digits) <= product(digits). */
seq_t a(idx_t n) {
    if (n == 1) return 0;                 /* the zero term */

    idx_t count = 1;                      /* count starts with the zero term */

    for (idx_t x = 1; x < 10000000ULL; ++x) {   /* brute force up to 7‑digit numbers */
        unsigned prev = 10;               /* larger than any digit */
        seq_t sum = 0;
        seq_t prod = 1;
        idx_t temp = x;
        bool ok = true;

        /* examine digits from least significant to most significant */
        while (temp > 0) {
            unsigned d = temp % 10;
            temp /= 10;

            /* enforce non‑decreasing digits (left‑to‑right) */
            if (d > prev) {               /* digits must not increase from right to left */
                ok = false;
                break;
            }
            prev = d;
            sum += (seq_t)d;
            prod *= (seq_t)d;
        }

        if (!ok) continue;                /* fails non‑decreasing test */

        if (sum <= prod) {                /* passes sum ≤ product test */
            ++count;
            if (count == n) return (seq_t)x;
        }
    }

    return 0;   /* should never be reached for n ≤ 10000 with this bound */
}
