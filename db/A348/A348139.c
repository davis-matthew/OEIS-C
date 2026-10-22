/*
 * A348139  Three-digit numbers abc such that the quadratic equation ax^2 + bx
 *          + c = 0 has a rational root.
 * https://oeis.org/A348139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 147, u32 n <= 147, u64 n <= 147,
 *            u128 n <= 147, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth A348139 term by brute–force search over all three‑digit
   numbers abc (a>0, 0≤b,c≤9) that satisfy:
        D = b^2 – 4ac   is a perfect square k^2
        k ≤ b
   The sequence is listed in increasing numeric order. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Pre‑allocate space for at most 9*10*10 = 900 candidates. */
    static seq_t values[900];
    static size_t count = 0;
    static bool built = false;

    if (!built) {
        /* Iterate over all possible digits in natural numeric order */
        for (idx_t a = 1; a <= 9; ++a) {
            for (idx_t b = 0; b <= 9; ++b) {
                for (idx_t c = 0; c <= 9; ++c) {
                    idx_t bb   = b * b;          /* b^2   */
                    idx_t t4ac = 4 * a * c;      /* 4ac   */
                    if (bb < t4ac) continue;    /* D < 0 → impossible */

                    idx_t D = bb - t4ac;         /* discriminant */

                    /* Integer square root for D (D ≤ 81). */
                    int k = 0;
                    while ((size_t)(k + 1) * (k + 1) <= D) ++k;

                    if ((idx_t)k * k != D) continue;  /* not a perfect square */
                    if ((idx_t)k > b) continue;       /* root > 0 would occur */

                    int num = (int)(a * 100 + b * 10 + c);  /* abc as integer */
                    values[count++] = (seq_t)num;
                }
            }
        }
        built = true;
    }

    /* Return the requested term if the index is within range. */
    if (n == 0 || n > count) return (seq_t)0;
    return values[n - 1];
}
