/*
 * A292309  Numbers equal to the sum of three triangular numbers in arithmetic
 *          progression.
 * https://oeis.org/A292309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*A000217(k) for some k that is the middle term of three
 *            triangular numbers in arithmetic progression
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* triangular number T_k = k*(k+1)/2 */
static inline seq_t tri(idx_t k)
{
    return ((seq_t)k * (k + 1)) / 2;
}

/*  a(n) – sequence A292309 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, but never called */

    idx_t found = 0;               /* number of terms found so far */
    idx_t b = 2;                   /* start with the first possible k */

    while (1) {
        /* T_b is the middle triangular number of a potential AP */
        const seq_t T_b = tri(b);
        const seq_t two_b = T_b + T_b;   /* 2*T_b */

        /* search for a c > b such that T_b + T_c is a perfect square
           with sqrt odd, giving an integer a < b. */
        idx_t c = b + 1;
        for (;;) {
            const seq_t T_c = tri(c);
            if (T_c > two_b) break;     /* would make the needed value negative */

            const seq_t sum_needed = two_b - T_c;          /* 2*T_b - T_c = T_a */
            const seq_t D          = sum_needed * (seq_t)8 + 1; /* discriminant */

            const seq_t s = floor_sqrt(D);
            if (s * s == D && (s & 1)) {                 /* sqrt must be odd */
                const idx_t a_idx = (idx_t)((s - 1) / 2);
                if (a_idx < b) {                         /* valid triple found */
                    ++found;
                    if (found == n) {
                        return T_b * (seq_t)3;             /* a(n) = 3*T_b   */
                    }
                    break;                                 /* next b */
                }
            }
            ++c;
        }
        ++b;
    }
}
