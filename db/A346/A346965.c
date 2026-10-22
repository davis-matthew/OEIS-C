/*
 * A346965  a(n) is the number of ascending subsequences in reducing n to 1
 *          using the Collatz reduction, or -1 if n refutes the Collatz
 *          conjecture.
 * https://oeis.org/A346965
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of maximal blocks of odd reductions *
 *            (ascending subsequences) in the Collatz trajectory from n to 1.
 *            * * Each block consists of one or more consecutive odd steps *
 *            (n → (3*n+1)/2). We count a new block whenever we encounter an
 *            odd * number that follows an even number or the st...
 * Range:     u8 n <= 26, u16 n <= 7658, u32 n <= 9803, u64 n <= 9803,
 *            u128 n <= 9803, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   // safety for accidental zero
    seq_t cur = (seq_t)n;
    seq_t cnt  = 0;
    bool in_asc = false;

    while (cur > 1) {
        if ((cur & 1) != 0) {          // odd step: start/continue ascending
            if (!in_asc) {             // beginning of a new ascending block
                ++cnt;
                in_asc = true;
            }
            cur = (cur * 3 + 1) / 2;    // combined 3*n+1 then /2 step
        } else {                       // even step: break ascending block
            in_asc = false;
            cur >>= 1;                 // divide by 2
        }
    }
    return cnt;
}
