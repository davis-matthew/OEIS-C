/*
 * A293822  Number of integer-sided pentagons having perimeter n, modulo
 *          rotations but not reflections.
 * https://oeis.org/A293822
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom_int_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) = number of integer‑sided pentagons with perimeter n,
   counted modulo rotations (reflections counted separately).
   The algorithm applies Burnside’s lemma: only non‑trivial rotations
   fix the all‑equal pentagons when n is divisible by 5.
   Fix(0) is the number of 5‑tuples of positive integers with sum n
   and each strictly less than n/2.  This is a bounded composition
   count solved by inclusion–exclusion.  */
#include <stdint.h>
#include "../utils/binom_int_int.h"

seq_t a(idx_t n) {
    if (n < 5) return 0;                     /* sequence starts at n = 5 */
    int IntN = (int)n;                       /* n fits in 32‑bit for our loop */
    int T = (IntN - 1) / 2;                  /* floor((n-1)/2) */
    int U = T - 1;                           /* upper bound for yi = ai-1 */
    int S = IntN - 5;                        /* sum of yi */
    seq_t count = 0;

    /* Inclusion–exclusion over variables exceeding U */
    for (int j = 0; j <= 5; ++j) {
        int rem = S - j * (U + 1);
        if (rem < 0) break;                 /* further j will only decrease rem */
        seq_t term = binom_int_int(5, j) *
                     binom_int_int(rem + 4, 4);
        if (j & 1)
            count -= term;
        else
            count += term;
    }

    seq_t total = count;
    if (n % 5 == 0) total += 4;              /* non‑trivial rotations fix equal sides */
    return total / 5;
}
