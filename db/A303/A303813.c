/*
 * A303813  Generalized 19-gonal (or enneadecagonal) numbers: m*(17*m - 15)/2
 *          with m = 0, +1, -1, +2, -2, +3, -3, ...
 * https://oeis.org/A303813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (34*n² + 60*n)/16 for n even; a(n) = (34*n² + 8*n –
 *            26)/16 for n odd (OEIS A303813: generalized 19‑gonal numbers)
 * Range:     u8 n <= 10, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* compute using 64‑bit intermediates to avoid overflow for
       small seq_t types and keep operations simple */
    unsigned long long nn = (unsigned long long)n;
    unsigned long long num;
    if (n & 1ULL) {
        /* odd n */
        num = 34ULL*nn*nn + 8ULL*nn - 26ULL;
    } else {
        /* even n */
        num = 34ULL*nn*nn + 60ULL*nn;
    }
    return (seq_t)(num / 16ULL);
}
