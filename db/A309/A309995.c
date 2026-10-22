/*
 * A309995  Balanced septenary enumeration (or balanced septenary
 *          representation) of integers; write n in septenary and then replace
 *          4's with (-3),s, 5's with (-2)'s, and 6's with (-1)'s.
 * https://oeis.org/A309995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, sign
 * Formula:   not stated
 * Range:     i8 n <= 146, i16 n <= 9494, i32 n <= 9494, i64 n <= 9494,
 *            i128 n <= 9494, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Balanced septenary representation:
   a(n) = 7 * a(n/7) + ((n%7) < 4 ? (n%7) : (n%7) - 7)
   The loop below evaluates this recurrence iteratively to avoid recursion. */
seq_t a(idx_t n)
{
    seq_t result = 0;   /* Accumulator for the integer value */
    seq_t power   = 1;   /* Current power of 7 corresponding to the digit position */

    while (n > 0)
    {
        /* Least‑significant base‑7 digit */
        idx_t r = n % 7;
        seq_t digit;

        /* Replace digits 4,5,6 by -3,-2,-1 respectively */
        if (r < 4)
            digit = (seq_t)r;
        else
            digit = -( (seq_t)(7 - r) );  /* 4→-3, 5→-2, 6→-1 */

        result += power * digit;   /* Add the contribution of this digit */
        power  *= 7;               /* Move to the next higher position */
        n      /= 7;               /* Discard the processed digit */
    }

    return result;
}
