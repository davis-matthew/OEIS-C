/*
 * A278996  Numbers of the form (3h+1)*3^(2k)-1 or (3h+2)*3^(2k+1)-1 for h,k
 *          in N.
 * https://oeis.org/A278996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9873 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A278996: numbers of the form (3h+1)*3^(2k)-1 or (3h+2)*3^(2k+1)-1.  
   Equivalent membership test: an integer n is in the sequence iff
   n mod 9 is 0,3,5,6, or n mod 9 is 8 and (n-8)/9 is in the sequence. */
#include <stdbool.h>

static inline bool a278996_in(seq_t x)
{
    while (x % 9 == 8) {
        x /= 9;
    }
    return (x % 9 == 0 || x % 9 == 3 || x % 9 == 5 || x % 9 == 6);
}

seq_t a(idx_t n)
{
    seq_t num = 0;
    idx_t count = 0;
    for (;;) {
        if (a278996_in(num)) {
            count++;
            if (count == n) return num;
        }
        num++;
    }
}
