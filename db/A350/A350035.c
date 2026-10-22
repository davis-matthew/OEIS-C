/*
 * A350035  Number of steps to reach 1 under repeated applications of the
 *          A350034 map, or -1 if 1 is never reached.
 * https://oeis.org/A350035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 7392, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of steps to reach 1 under the map:
 *   if n is odd:
 *      if n % 3 == 0 : n = n / 3
 *      else           : n = 5 * n + 1
 *   else (n is even):
 *      if n % 6 == 0 : n = n / 6
 *      else           : n = n / 2
 *   Count steps until n becomes 1. Return -1 if 1 is never reached.
 * Offset: 1   (a(1) = 0)
 */

seq_t a(idx_t n)
{
    if (n == 1) return 0;          /* a(1) = 0 */

    seq_t m   = (seq_t)n;          /* use seq_t for the map value */
    seq_t step = 0;

    /* this simple map never generates a loop that doesn't hit 1,
       but we guard against accidental infinite loops with a sane
       iteration limit. */
    const seq_t MAX_ITERS = 2000000;

    while (m > 1 && step < MAX_ITERS) {
        ++step;
        if (m & 1U) {               /* odd */
            if (m % 3 == 0)
                m /= 3;
            else
                m = 5 * m + 1;
        } else {                    /* even */
            if (m % 6 == 0)
                m /= 6;
            else
                m /= 2;
        }
    }

    if (m != 1) return (seq_t)-1;    /* didn't reach 1 within limit */
    return step;
}
