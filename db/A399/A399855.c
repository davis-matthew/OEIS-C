/*
 * A399855  a(n) is the least k > 1 such that the factorial base expansion of
 *          n appears as a substring in that of k*n.
 * https://oeis.org/A399855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 358, u32 n <= 5040, u64 n <= 5040,
 *            u128 n <= 5040, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for OEIS A399855: the least k>1 such that the
   factorial‑base expansion of n appears as a substring in that of k*n. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Convert a number into its factorial‑base representation.
   The digits are stored in most‑significant‑first order.
   `out` must point to a buffer of at least 32 uint8_t elements
   (sufficient for all n ≤ 10^4).  The length of the representation
   is written to `*len`. */
static void factorial_digits(seq_t num, uint8_t *out, size_t *len)
{
    if (num == 0) {
        *len = 0;
        return;
    }
    size_t pos = 0;
    size_t i = 1;                  /* i! place (i starts at 1) */
    while (num > 0) {
        seq_t div = (seq_t)(i + 1);            /* divisor for i!   */
        out[pos++] = (uint8_t)(num % div);     /* digit for i!     */
        num /= div;                            /* move to next place */
        ++i;
    }
    /* reverse to most‑significant‑first order */
    for (size_t j = 0; j < pos / 2; ++j) {
        uint8_t tmp = out[j];
        out[j] = out[pos - 1 - j];
        out[pos - 1 - j] = tmp;
    }
    *len = pos;
}

seq_t a(idx_t n)
{
    /* Special case n = 0: factorial expansion is empty. Any k>1 works; return 2. */
    if (n == 0) return (seq_t)2;

    /* Pre‑compute factorial digits of n. */
    uint8_t dn[32];
    size_t len_n;
    factorial_digits((seq_t)n, dn, &len_n);

    /* Search for the least k > 1. */
    for (idx_t k = 2; ; ++k) {
        seq_t prod = (seq_t)k * (seq_t)n;          /* compute k*n */

        uint8_t dk[32];
        size_t len_k;
        factorial_digits(prod, dk, &len_k);

        if (len_k < len_n) continue;              /* substring impossible */

        /* Scan all possible starting positions. */
        for (size_t pos = 0; pos + len_n <= len_k; ++pos) {
            bool match = true;
            for (size_t i = 0; i < len_n; ++i) {
                if (dk[pos + i] != dn[i]) {
                    match = false;
                    break;
                }
            }
            if (match) return (seq_t)k;
        }
    }
}
