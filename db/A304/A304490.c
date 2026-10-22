/*
 * A304490  a(1) = a(2) = a(3) = 1, a(4) = 5, a(5) = 6, a(6) = 4; a(n) =
 *          a(n-a(n-2)) + a(n-a(n-4)) for n > 6.
 * https://oeis.org/A304490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-a(n-2)) + a(n-a(n-4)) with initial values
 *            a(1)=a(2)=a(3)=1, a(4)=5, a(5)=6, a(6)=4. For n > 6 the sequence
 *            satisfies a quasi‑periodic pattern: a(6k) = 5, a(6k+1) = 6,
 *            a(6k+2) = a(6k+3) = 6k, a(6k+4) = 6k+3, a(6k+5) = 6k+4 (for k ≥
 *            1, except n = 7 which equals 5)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 6) {
        switch ((idx_t)n) {
            case 1: case 2: case 3: return (seq_t)1;
            case 4: return (seq_t)5;
            case 5: return (seq_t)6;
            case 6: return (seq_t)4;
        }
    }

    /* n = 7 is a special case (cannot use the periodic formula) */
    if (n == 7) return (seq_t)5;

    idx_t k = n / 6;
    idx_t r = n % 6;

    switch (r) {
        case 0: return (seq_t)5;                 /* a(6k)   */
        case 1: return (seq_t)6;                 /* a(6k+1) */
        case 2: return (seq_t)6 * k;             /* a(6k+2) */
        case 3: return (seq_t)6 * k;             /* a(6k+3) */
        case 4: return (seq_t)6 * k + (seq_t)3;  /* a(6k+4) */
        case 5: return (seq_t)6 * k + (seq_t)4;  /* a(6k+5) */
        default: return (seq_t)0;                /* unreachable */
    }
}
