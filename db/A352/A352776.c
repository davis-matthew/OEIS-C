/*
 * A352776  Numbers k such that w(k + w(k)) = w(k), where w(k) is the binary
 *          weight of k, A000120(k).
 * https://oeis.org/A352776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) = n-th nonnegative integer k such that
   popcount(k + popcount(k)) == popcount(k)   (k is sorted increasingly).
   a(1) = 0, a(2) = 1, a(3) = 3, a(4) = 10, …   */
/*   Here popcount(x) is the binary weight of x. */

#include <stdbool.h>

/* Count the number of set bits in an arbitrary-size unsigned integer.
   Works for all supported seq_t types, including 8/16/32/64/128-bit
   and arbitrary-precision integer classes with bitwise operators. */
static unsigned int popcount_seq_t(seq_t x)
{
    unsigned int c = 0;
    while (x) {
        x &= x - 1;   /* clear lowest set bit */
        c++;
    }
    return c;
}

seq_t a(idx_t n)
{
    /* n is 1-based index into the sequence */
    idx_t found = 0;
    seq_t k = 0;

    while (true) {
        unsigned int w = popcount_seq_t(k);          /* w(k)   */
        seq_t kp = k + (seq_t)w;                     /* k + w(k) */
        if (popcount_seq_t(kp) == w) {               /* w(k+ w(k)) == w(k) */
            found++;
            if (found == n) {
                return k;
            }
        }
        k++; /* next candidate */
    }
}
