/*
 * A296734  Number of nX3 0..1 arrays with each 1 adjacent to 1, 2 or 4
 *          king-move neighboring 1s.
 * https://oeis.org/A296734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence for OEIS sequence A296734:
   a(1)=4; a(2)=31; a(3)=126; a(4)=575; a(5)=2832; a(6)=13322;
   a(7)=62874; a(8)=299056; a(9)=1418177; a(10)=6723333;
   a(11)=31896327; a(12)=151295253; a(13)=717601109;
   a(14)=3403806391; a(15)=16145223273; a(16)=76580830416;
   For n>=17:
     a(n) = 4*a(n-1)+3*a(n-2)+14*a(n-3)-42*a(n-4)-56*a(n-5)-51*a(n-6)
            +118*a(n-7)+189*a(n-8)+60*a(n-9)-114*a(n-10)-187*a(n-11)
            -25*a(n-12)+82*a(n-13)+33*a(n-14)-12*a(n-15)-4*a(n-16)
*/
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Initial 16 terms, indexed 0..15 for a(1)..a(16). */
    static const seq_t init[16] = {
        (seq_t)4ULL, (seq_t)31ULL, (seq_t)126ULL, (seq_t)575ULL,
        (seq_t)2832ULL, (seq_t)13322ULL, (seq_t)62874ULL, (seq_t)299056ULL,
        (seq_t)1418177ULL, (seq_t)6723333ULL, (seq_t)31896327ULL,
        (seq_t)151295253ULL, (seq_t)717601109ULL, (seq_t)3403806391ULL,
        (seq_t)16145223273ULL, (seq_t)76580830416ULL
    };

    if (n <= 16)
        return init[n - 1];

    /* Buffer holding the last 16 computed values:
       buf[0] = a(n-16), buf[15] = a(n-1). */
    seq_t buf[16];
    for (idx_t i = 0; i < 16; ++i)
        buf[i] = init[i];

    /* Compute terms from 17 up to n. */
    for (idx_t k = 17; k <= n; ++k) {
        seq_t new_val =
            buf[15] * (seq_t)4
          + buf[14] * (seq_t)3
          + buf[13] * (seq_t)14
          - buf[12] * (seq_t)42
          - buf[11] * (seq_t)56
          - buf[10] * (seq_t)51
          + buf[ 9] * (seq_t)118
          + buf[ 8] * (seq_t)189
          + buf[ 7] * (seq_t)60
          - buf[ 6] * (seq_t)114
          - buf[ 5] * (seq_t)187
          - buf[ 4] * (seq_t)25
          + buf[ 3] * (seq_t)82
          + buf[ 2] * (seq_t)33
          - buf[ 1] * (seq_t)12
          - buf[ 0] * (seq_t)4;

        /* Shift left: drop a(n-16), keep a(n-15)...a(n-1), append new_val. */
        for (idx_t i = 0; i < 15; ++i)
            buf[i] = buf[i + 1];
        buf[15] = new_val;
    }

    return buf[15];
}
