/*
 * A353907  Numbers k such that k equals {alternating sum of digits of k}
 *          raised to the power of {number of digits of k}.
 * https://oeis.org/A353907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 9, u32 n <= 9, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   digit_len, get_digits, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A353907 – Numbers k such that
   k = (alternating sum of digits of k) raised to the power
   of (number of digits of k).  */
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_mul.h"
#include "../utils/get_digits.h"
#include "../utils/digit_len.h"

/* Store the computed terms in ascending order.  The sequence
   is known to be finite (only the values found by the
   following search).  The table is filled lazily on the first
   call to a(). */
static seq_t      terms[64];
static size_t     term_count = 0;

/* Compute the alternating sum of the decimal digits of x.
   The digit buffer returned by get_digits() contains the most
   significant digit first. */
static int alt_sum(seq_t x)
{
    uint8_t  buf[128];
    size_t   len = 0;
    get_digits(x, buf, &len);
    int s = 0;
    for (size_t i = 0; i < len; ++i)
        s += ((i & 1) == 0) ? buf[i] : -buf[i];
    return s;
}

/* Generate all terms of the sequence that fit into seq_t.
   For each possible alternating‑sum value s = 1…9 we
   iterate over exponents d and test the two conditions:
        1. the number of decimal digits of s^d equals d;
        2. the alternating sum of those digits equals s. */
static void compute_terms(void)
{
    if (term_count > 0)   /* already initialized */
        return;

    for (int s = 1; s <= 9; ++s) {
        seq_t val = (seq_t)s;          /* s^1 */
        for (idx_t d = 1; d <= 200; ++d) {
            size_t digits = digit_len(val);
            if (digits < (size_t)d)      /* will never match again */
                break;

            if (digits == (size_t)d && alt_sum(val) == s)
                terms[term_count++] = val;

            /* Next power: s^(d+1) */
            val = seq_mul(val, (seq_t)s);
        }
    }
}

/* Public accessor for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 0 */

    compute_terms();

    if (n <= (idx_t)term_count)
        return terms[n - 1];
    else
        return (seq_t)0;          /* no further terms known */
}
