/*
 * A280731  Lexicographically least strictly increasing sequence such that,
 *          for any n>0, Sum_{k=1..n} a(k) can be computed without carries in
 *          base 9 (the numbers are written in base 10).
 * https://oeis.org/A280731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Lexicographically least strictly increasing sequence such that *
 *            for any n>0, Sum_{k=1..n} a(k) can be computed without carries
 *            in base 9. * * Initial terms: * a(1)=1, a(2)=2, a(3)=3, a(4)=9,
 *            a(5)=10, a(6)=18, a(7)=19 * Recurrence for n>7: * a(n)=9*a(n-4)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 41, u64 n <= 81, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init_seq[8] = {0, 1, 2, 3, 9, 10, 18, 19};

static seq_t pow9(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 9;
    while (exp) {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)                    /* undefined for offset 1; return 0 */
        return 0;
    if (n <= 7)                    /* initial terms */
        return init_seq[n];

    /* Number of times we multiply by 9 (k in recurrence) */
    idx_t k   = (n - 8) / 4 + 1;
    /* Base index after reducing by multiples of 4 */
    idx_t bi  = n - 4 * k;  /* 4 <= bi <= 7 for n > 7 */
    seq_t base_val = init_seq[bi];

    return pow9(k) * base_val;
}
