/*
 * A350044  Loop starting at 187 in the Collatz-like map {x -> 3x+5 if x is
 *          odd, x/2 otherwise}.
 * https://oeis.org/A350044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 187, a(n) = A181762(a(n-1)) for n>1, where A181762(x) =
 *            3x+5 if x odd, x/2 if x even
 * Range:     u8 none, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of the 44‑term cycle */
    static seq_t cycle[64];
    static size_t cycle_len = 0;
    static bool initialized = false;

    if (!initialized) {
        seq_t x = (seq_t)187;
        size_t i = 0;
        do {
            cycle[i++] = x;
            if (x & (seq_t)1) {           /* odd */
                x = x * (seq_t)3 + (seq_t)5;
            } else {                      /* even */
                x = x / (seq_t)2;
            }
        } while (x != (seq_t)187);
        cycle_len = i;                     /* should be 44 */
        initialized = true;
    }

    size_t idx = (size_t)((n - (idx_t)1) % (idx_t)cycle_len);
    return cycle[idx];
}
