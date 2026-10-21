/*
 * A256882  Numbers divisible by prime(d+1) for each digit d of their base-2
 *          representation.
 * https://oeis.org/A256882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers divisible by prime(d+1) for each binary digit d of their
 * base‑2 representation.  For a binary number n>0 let
 *
 *   has_zero = does n contain a 0 bit in its binary length ?
 *
 *   • if has_zero is true (i.e. n contains both 0 and 1 bits) then n must
 *     be divisible by both 2 (prime(1)) and 3 (prime(2)), i.e. n % 6 == 0;
 *
 *   • otherwise (n consists solely of 1 bits) n must be divisible by 3
 *     (prime(2)), i.e. n % 3 == 0;
 *
 *   0 is accepted by definition.
 *
 * The function a(n) returns the n‑th term (offset 1).  It is implemented
 * by brute‑force searching the non‑negative integers and collecting those
 * that satisfy the above conditions.  This works fast enough for n up to
 * about 10 000 (at most a few tens of thousands of integer tests).  */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t  count = 0;          /* how many terms found so far          */
    seq_t  cur   = 0;          /* current integer being tested         */

    while (count < n) {
        bool ok;
        if (cur == 0) {
            ok = true; /* 0 is accepted */
        } else {
            /* Check whether cur contains a zero bit among its binary
               digits (other than the implicit leading zeros).            */
            bool has_zero = false;
            seq_t temp = cur;
            while (temp > 0) {
                if ((temp & 1) == 0) {
                    has_zero = true;
                }
                temp >>= 1;
            }

            if (has_zero) {
                /* must be divisible by both 2 and 3 => divisible by 6 */
                ok = (cur % 6 == 0);
            } else {
                /* only 1 bits: must be divisible by 3          */
                ok = (cur % 3 == 0);
            }
        }

        if (ok) {
            count++;
            if (count == n) {
                return cur;
            }
        }
        cur++;
    }

    /* In theory we should never reach here because the while loop
       guarantees that we return once count == n. */
    return 0;
}
