/*
 * A321011  Trajectory of 86 under repeated application of the map k ->
 *          A320486(k^2).
 * https://oeis.org/A321011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   remove_nonunique_digits, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=86, a(n+1)=remove_nonunique_digits(a(n)^2).  */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_mul.h"
#include "../utils/remove_nonunique_digits.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0; /* undefined for offset 0 */

    static seq_t cycle[10];
    static bool initialized = false;

    if (!initialized) {
        cycle[0] = (seq_t)86;
        for (int i = 1; i < 10; ++i) {
            seq_t sq = seq_mul(cycle[i - 1], cycle[i - 1]);   /* k^2 */
            cycle[i] = remove_nonunique_digits(sq);
        }
        initialized = true;
    }

    idx_t pos = (n - (idx_t)1) % (idx_t)10;
    return cycle[(int)pos];
}
