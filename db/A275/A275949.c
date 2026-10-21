/*
 * A275949  Number of distinct nonzero digits that occur multiple times in
 *          factorial base representation of n: a(n) = A056170(A275735(n)).
 * https://oeis.org/A275949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct nonzero digits that occur more than
 *            once in the factorial base representation of n
 * Range:     u8 n <= 9819, u16 n <= 9819, u32 n <= 9819, u64 n <= 9819,
 *            u128 n <= 9819, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The algorithm converts n into factorial base (factoradic) using
   repeated division by increasing bases (k+1). While doing so, it counts
   the frequency of each non‑zero digit. The result is the number of digits
   that appear at least twice. This method runs in O(#digits) which is
   O(log n) for n ≥ 0. */

/* Include type headers that are typically provided by the build system. */
#include <stdint.h>

/* Use a small static array big enough for all possible digits.
   For n up to (2^64−1) the number of digits is < 21, and the value
   of any digit is always ≤ digit index. 20 is more than enough. */
#define MAX_DIGITS 20

seq_t a(idx_t n)
{
    /* Frequency table for digits 0..MAX_DIGITS-1.  Zero digits are ignored. */
    unsigned char freq[MAX_DIGITS] = {0};

    idx_t k = 1;               /* Current factorial base index (1! = 1). */
    /* Keep converting until the remaining number is zero. */
    while (n > 0) {
        idx_t digit = n % (k + 1);   /* 0 ≤ digit ≤ k. */
        if (digit > 0 && digit < MAX_DIGITS)
            freq[digit]++;            /* Count only non‑zero digits. */
        n /= (k + 1);
        ++k;
    }

    /* Count how many distinct digits appear more than once. */
    unsigned char result = 0;
    for (unsigned int d = 1; d < MAX_DIGITS; ++d) {
        if (freq[d] > 1)
            ++result;
    }

    /* Return the result in the required seq_t type. */
    return (seq_t)result;
}
