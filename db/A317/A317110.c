/*
 * A317110  Numbers k such that the smallest digit of k is equal to the
 *          additive digital root of k (in base 10).
 * https://oeis.org/A317110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A317110: numbers k such that the smallest digit of k equals its additive digital root (base 10). */

#ifndef __cplusplus
#include <stdbool.h>
#else
/* In C++ the bool type is already built‑in. */
#endif

/* Test whether num satisfies the condition. */
static int is_valid(seq_t num)
{
    /* 0 is always valid. */
    if (num == 0) return 1;

    /* Find the smallest decimal digit of num. */
    int min_d = 9;
    seq_t tmp = num;
    while (tmp > 0) {
        int d = (int)(tmp % 10);
        if (d < min_d) {
            min_d = d;
            if (min_d == 0) break;   /* cannot become smaller */
        }
        tmp = tmp / 10;
    }

    /* Compute the additive digital root of num. */
    seq_t root = ((num - 1) % 9) + 1;  /* for num > 0; result is 1..9 */

    return (root == min_d) ? 1 : 0;
}

/* Return the n-th element of A317110 (1‑based index). */
seq_t a(idx_t n)
{
    idx_t  count = 0;
    seq_t  candidate = 0;

    while (1) {
        if (is_valid(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
}
