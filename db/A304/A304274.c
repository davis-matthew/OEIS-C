/*
 * A304274  The concatenation of the first n elements is the largest positive
 *          even number with n digits when written in base 3/2.
 * https://oeis.org/A304274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 40, u32 n <= 81, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A304274
   a(n) = largest digit of the largest positive even integer
   with n digits in base 3/2.
   The algorithm follows the recurrence:
       E_1 = 2
       a(n) = 2 if (E_{n-1} mod 4) == 0     // largest even digit
              1 otherwise                   // largest odd digit
       E_n = (3*E_{n-1})/2 + a(n)
   This produces the sequence:
       2,1,2,2,1,1,1,2,2,1,2,1,1,2,2,1,2,1,2,1,1,2,2,1,...
   The function works for any n≥1 in the supported build types.
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Sequence offset is 1; behaviour for n==0 is undefined. */
        return (seq_t)0;
    }

    seq_t E = (seq_t)2;          /* E_1 = 2   */
    if (n == 1) return (seq_t)2; /* a(1) = 2 */

    seq_t digit;
    for (idx_t i = 2; i <= n; ++i) {
        /* E is even.  E mod 4 decides the parity of (3/2)*E. */
        if ((E & (seq_t)2) == 0)
            digit = (seq_t)2; /* choose 2 if (3/2)*E is even   */
        else
            digit = (seq_t)1; /* otherwise choose 1           */

        if (i == n)
            return digit;     /* value needed for position n */

        /* Update E for the next iteration */
        E = (E * (seq_t)3) / (seq_t)2 + digit;
    }

    /* Should never reach here */
    return (seq_t)0;
}
