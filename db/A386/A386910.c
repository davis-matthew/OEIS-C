/*
 * A386910  Number of iterations of seven segments count x -> A063720(x) to go
 *          from n to a fixed point.
 * https://oeis.org/A386910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A386910 – number of iterations of the 7‑segment digit‑sum
   function A063720 until a fixed point (4 or 5) is reached.
   The mapping of digits to segment counts is:
      0 → 6 , 1 → 2 , 2 → 5 , 3 → 5 , 4 → 4 , 5 → 5 ,
      6 → 5 , 7 → 3 , 8 → 7 , 9 → 5
   This mapping reproduces all listed values of the sequence.
*/

#include <stdint.h>

static const unsigned char SEG[10] = {6, 2, 5, 5, 4, 5, 5, 3, 7, 5};

/* Sum of 7‑segment counts of the decimal digits of n. */
static seq_t segsum(seq_t n)
{
    seq_t sum = 0;
    if (n == 0) {
        return (seq_t)SEG[0];
    }
    while (n > 0) {
        unsigned int d = (unsigned int)(n % (seq_t)10);
        sum += (seq_t)SEG[d];
        n /= (seq_t)10;
    }
    return sum;
}

/* Required entry point. */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    if (val == 4 || val == 5) {
        return (seq_t)0;
    }
    seq_t steps = 0;
    while (1) {
        val = segsum(val);
        ++steps;
        if (val == 4 || val == 5) {
            break;
        }
    }
    return steps;
}
