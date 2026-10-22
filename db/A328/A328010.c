/*
 * A328010  The 5x + 1 sequence beginning at 17.
 * https://oeis.org/A328010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 5x + 1 sequence starting at 17:
 * a(0) = 17
 * a(n+1) = 5*a(n) + 1  if a(n) is odd
 *          a(n)/2     if a(n) is even.
 */
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache the first 10 terms, which form the period */
    static bool initialized = false;
    static seq_t cache[10];

    if (!initialized) {
        seq_t val = (seq_t)17;
        cache[0] = val;
        for (idx_t i = 1; i < 10; ++i) {
            if ((val & 1) != 0)          /* val is odd */
                val = val * (seq_t)5 + (seq_t)1;
            else                         /* val is even */
                val = val >> 1;          /* divide by 2 */
            cache[i] = val;
        }
        initialized = true;
    }

    if (n < 10)
        return cache[n];
    else
        return cache[n % 10];
}
