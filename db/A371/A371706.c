/*
 * A371706  a(n) is the least k > 0 such that n^k contains the digit 1.
 * https://oeis.org/A371706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 6369, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_frequencies, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = the least k > 0 such that the decimal
 * representation of n^k contains the digit 1.  The function
 * uses the provided big‑integer utility 'digit_frequencies'
 * to test for the presence of the digit 1, and 'seq_mul'
 * to perform powers iteratively.  No assumptions are made
 * about the array ordering of successive calls. */
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/digit_frequencies.h"

/* Return true iff the decimal digits of x contain a '1'. */
static bool contains_one(seq_t x)
{
    unsigned int freq[10] = {0};
    /* digit_frequencies converts x to its base‑10 digits
     * and fills 'freq' with their frequencies. */
    digit_frequencies(x, freq);
    return freq[1] != 0;
}

seq_t a(idx_t n)
{
    /* Convert the index to a seq_t for arithmetic. */
    seq_t base = (seq_t)n;

    /* k = 1 : check n^1 immediately. */
    if (contains_one(base))
        return (seq_t)1;

    seq_t cur = base;   /* cur holds n^k for the current k. */
    seq_t k   = 1;      /* Current exponent (already used). */

    for (;;) {
        /* Multiply to get n^(k+1). */
        cur = seq_mul(cur, base);
        ++k;
        if (contains_one(cur))
            return k;
    }
}
