/*
 * A252389  Number of (5+2) X (n+2) 0..3 arrays with every 3 X 3 subblock row
 *          and diagonal sum equal to 0 3 5 6 or 7 and every 3 X 3 column and
 *          antidiagonal sum not equal to 0 3 5 6 or 7.
 * https://oeis.org/A252389
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 17, u32 n <= 52, u64 n <= 121, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 4*a(n-3) - 4*a(n-6) + a(n-9)   for n>14
 * Initial values (a(1)..a(14)):
 *  a(1)  = 785
 *  a(2)  = 637
 *  a(3)  = 799
 *  a(4)  = 916
 *  a(5)  = 1305
 *  a(6)  = 1894
 *  a(7)  = 2310
 *  a(8)  = 3280
 *  a(9)  = 4791
 *  a(10) = 5934
 *  a(11) = 8451
 *  a(12) = 12386
 *  a(13) = 15413
 *  a(14) = 21987
 *
 * The function implements this recurrence with a small circular
 * buffer to avoid dynamic allocation, making it valid C11 and
 * C++17.  It computes values up to the requested index n
 * (assumed to be ≥1) and returns a(n) as seq_t.
 */

#include <stdint.h>

/* The recurrence depends only on the 9 most recent terms,
 * so a buffer of 15 elements guarantees that a value is never
 * overwritten before it is needed again. */
static const seq_t init[15] = {
    0ull,        /* 0 (unused) */
    785ull,      /* 1 */
    637ull,      /* 2 */
    799ull,      /* 3 */
    916ull,      /* 4 */
    1305ull,     /* 5 */
    1894ull,     /* 6 */
    2310ull,     /* 7 */
    3280ull,     /* 8 */
    4791ull,     /* 9 */
    5934ull,     /* 10 */
    8451ull,     /* 11 */
    12386ull,    /* 12 */
    15413ull,    /* 13 */
    21987ull     /* 14 */
};

/* Compute a(n) using the recurrence.  The buffer holds the
 * most recent 15 computed values so that a(n-3), a(n-6),
 * and a(n-9) can be retrieved without overwriting them
 * before they are needed. */
seq_t a(idx_t n)
{
    if (n <= 14) {          /* immediate answer for the base cases */
        return init[n];
    }

    seq_t buf[15];
    /* initialise the buffer with the first 15 terms */
    for (idx_t i = 0; i < 15; ++i) {
        buf[i] = init[i];
    }

    /* compute a(15) .. a(n) iteratively */
    for (idx_t i = 15; i <= n; ++i) {
        seq_t term = 4 * buf[(i - 3) % 15];
        term -= 4 * buf[(i - 6) % 15];
        term += buf[(i - 9) % 15];
        buf[i % 15] = term;
    }

    return buf[n % 15];
}
