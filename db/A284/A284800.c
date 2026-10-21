/*
 * A284800  Fixed points of the transform A284799.
 * https://oeis.org/A284800
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A284800: Fixed points of the base‑4 complement‑reverse transform.
   A number x satisfies: rev(complement_base4(x)) = x.
   The sequence is the sorted list of such numbers starting with 6. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

/* Return the n-th term (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Find the minimal k such that 4^k > n.
       4^k is the number of possible first‑half patterns of length k. */
    idx_t powPrev = 1;   /* 4^(k-1) after the loop */
    idx_t k = 0;         /* current exponent (k-1) */
    while (powPrev <= n / 4) {
        powPrev *= 4;
        ++k;
    }
    ++k;                 /* now k is the exponent with 4^k > n */

    /* powPrev now equals 4^(k-1) */
    const idx_t halfCount = powPrev;     /* number of patterns for a given first digit */
    idx_t t = n - halfCount;             /* zero‑based index within this block */

    /* Determine the first digit (1,2,3) and the offset within that sub‑block. */
    idx_t firstDigitIndex = t / halfCount;   /* 0,1,2 */
    idx_t firstDigit = firstDigitIndex + 1;   /* actual digit value 1..3 */
    idx_t remainder = t % halfCount;          /* remainder for the remaining k-1 digits */

    /* Build the first half digits. */
    size_t len = (size_t)k;
    /* A small fixed buffer is sufficient: even for n up to 2^64 the required
       number of base‑4 digits is at most 64, so 128 is ample. */
    unsigned char digits[128] = {0};

    digits[0] = (unsigned char)firstDigit;
    /* Fill digits[1 .. k-1] from most significant to least by repeatedly
       extracting base‑4 digits of the remainder. */
    for (size_t i = len; i-- > 1; ) {
        digits[i] = (unsigned char)(remainder % 4);
        remainder /= 4;
    }

    /* Construct the full number from its digits. */
    seq_t result = 0;
    /* First half, most significant to least significant. */
    for (size_t i = 0; i < len; ++i) {
        result = result * 4 + (seq_t)digits[i];
    }
    /* Second half: complement and reverse the first half. */
    for (size_t i = len; i-- > 0; ) {
        result = result * 4 + (seq_t)(3 - digits[i]);
    }

    return result;
}
