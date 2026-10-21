/*
 * A248910  Numbers with no zeros in base-6 representation.
 * https://oeis.org/A248910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 155, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A248910: numbers with no zeros in base‑6 representation */

/* a(n) returns the n-th positive integer whose base‑6 digits are all non‑zero. */
seq_t a(idx_t n)
{
    /* Find the digit length L such that the cumulative count of numbers
       with up to L digits (in base‑6, digits 1–5) reaches or exceeds n. */
    idx_t sum   = 0;          /* cumulative count of terms of length < current L   */
    idx_t pow5  = 5;          /* 5^L  (current block size, starting at 5^1)       */
    int   L     = 0;          /* number of blocks processed; L+1 = digit length */

    /* While n is greater than the total count of numbers with up to L digits */
    while (1) {
        idx_t diff = n - sum;          /* n - sum ≤ pow5  implies we are in length L+1 */
        if (diff <= pow5)
            break;
        sum += pow5;                   /* include block of length L+1 */
        /* Multiply by 5 safely, using comparison to avoid overflow */
        if (pow5 > (UINT64_MAX / 5))
            pow5 = UINT64_MAX;         /* further multiplication would overflow;
                                        but we already have enough length   */
        else
            pow5 *= 5;
        ++L;
    }

    int  len   = L + 1;                 /* actual number of base‑6 digits       */
    idx_t pos  = n - sum - 1;           /* zero‑based index among len‑digit strings */

    /* Convert pos to base‑5 representation with exactly len digits. */
    unsigned char digit5[64] = {0};      /* len ≤ 64 is safe for any practical n    */
    for (int i = len-1; i >= 0; --i) {
        digit5[i] = (unsigned char)(pos % 5);
        pos /= 5;
    }

    /* Build the base‑6 number by adding 1 to each base‑5 digit and
       interpreting in base 6 (most significant first). */
    seq_t result = 0;
    for (int i = 0; i < len; ++i) {
        result = result * 6 + (seq_t)(digit5[i] + 1);
    }
    return result;
}
