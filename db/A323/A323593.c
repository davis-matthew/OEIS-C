/*
 * A323593  Position on a Dvorak typewriter (or computer) keyboard where the
 *          n-th letter of the alphabet can be found.
 * https://oeis.org/A323593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = dvorak_pos[(n-1) % 26]   (periodic mapping of Dvorak keyboard) */
#include <stdint.h>

/* Dvorak keyboard key numbers for letters A–Z */
static const seq_t dvorak_pos[26] = {
    8, 22, 5, 13, 10, 3, 4, 14,   /* A–H */
    12, 19, 20, 7, 23, 16, 9, 1,  /* I–P */
    18, 6, 17, 15, 11, 25, 14, 21, /* Q–X */
    2, 26                               /* Y–Z */
};

seq_t a(idx_t n)
{
    /* n ≥ 1, so n-1 is safe; mod 26 gives 0‑based index */
    idx_t idx = (n - 1) % 26ULL;
    return dvorak_pos[idx];
}
