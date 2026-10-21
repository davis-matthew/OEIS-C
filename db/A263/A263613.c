/*
 * A263613  Palindromic numbers in base 4 that are cubes.
 * https://oeis.org/A263613
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Palindromic numbers in base 4 that are perfect cubes.
   The sequence is represented using decimal notation of the
   base‑4 palindrome digits.
   a(1) = 0
   a(2) = 1
   for n >= 3   let k = n-2
   the base‑4 digits of a(n) are:
     1, (k zeroes), 3, (k-1 zeroes), 3, (k-1 zeroes), 1
   i.e. positions 0, k, 2k, 3k have digits 1,3,3,1 respectively
   and all other positions are 0.
*/

#include <stdlib.h>
#include "../utils/seq_from_str.h"    /* seq_from_str(const char *s) returns seq_t */

seq_t a(idx_t n)
{
    if (n == 1) {
        return seq_from_str("0");
    }
    if (n == 2) {
        return seq_from_str("1");
    }

    idx_t k = n - 2;                      /* k >= 1 for n >= 3 */
    size_t len = 3 * k + 1;               /* number of decimal digits */

    char *buf = (char *)malloc(len + 1);   /* +1 for terminating '\0' */
    if (!buf) {
        /* In practice the caller must handle memory failure */
        exit(1);
    }

    /* initialise all digits to '0' */
    for (size_t i = 0; i < len; ++i) {
        buf[i] = '0';
    }

    /* set the non‑zero digits */
    buf[0]          = '1';              /* most significant digit (pos 3k) */
    buf[(size_t)k]  = '3';              /* digit at position 2k (from high) */
    buf[(size_t)2 * k] = '3';           /* digit at position k   (from high) */
    buf[len - 1]    = '1';              /* least significant digit (pos 0) */

    buf[len] = '\0';                    /* null terminate */

    seq_t result = seq_from_str(buf);
    free(buf);
    return result;
}
