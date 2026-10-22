/*
 * A394793  a(n) is the smallest positive multiple of n whose decimal
 *          representation contains every distinct digit of n+1.
 * https://oeis.org/A394793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Brute‑force search for the smallest positive multiple of n
   whose decimal representation contains every distinct digit
   of (n+1).  */
/* This implementation is valid for both the 64‑bit build and
   the arbitrary‑precision build (C++17). */

static inline unsigned digit_mask(seq_t x)
{
    unsigned m = 0;
    if (x == 0)                 /* digit 0 is present */
        return 1u;
    while (x > 0) {
        unsigned d = (unsigned)(x % 10);
        m |= 1u << d;
        x /= 10;
    }
    return m;
}

seq_t a(idx_t n)
{
    if (n == 0)                 /* safety: sequence is 1‑based, but avoid UB */
        return 0;

    seq_t N = (seq_t)n;                 /* convert index to seq_t once */
    seq_t n1 = N + 1;                   /* n+1 whose digits are required */
    unsigned required = digit_mask(n1);

    for (seq_t m = N; ; m += N) {       /* iterate over multiples of n */
        if ((digit_mask(m) & required) == required)
            return m;
    }
}
