/*
 * A305826  Number of vectors in {0..4}^n with sum divisible by 9.
 * https://oeis.org/A305826
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
   Recurrence: let count[r] be the number of vectors of the current length
   with sum ≡ r (mod 9).  Initially count[0] = 1 and the rest 0.
   For each new position, we add a value x ∈ {0,1,2,3,4} and
   update the remainders:
       new[(r + x) % 9] += count[r].
   After processing n positions, the answer is count[0].
*/
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t cur[9] = {0}, next[9];
    cur[0] = 1;                         // a(0) = 1

    for (idx_t i = 1; i <= n; ++i) {
        for (int r = 0; r < 9; ++r) next[r] = 0;
        for (int r = 0; r < 9; ++r) {
            seq_t val = cur[r];
            if (!val) continue;
            /* add values 0..4 to remainder r */
            next[r] += val;                 // +0
            next[(r + 1) % 9] += val;       // +1
            next[(r + 2) % 9] += val;       // +2
            next[(r + 3) % 9] += val;       // +3
            next[(r + 4) % 9] += val;       // +4
        }
        for (int r = 0; r < 9; ++r) cur[r] = next[r];
    }
    return cur[0];
}
