/*
 * A328933  For any negative number, add the digits (assigning the negative
 *          sign just to the first digit), square the result and add it to the
 *          original number. This sequence shows negative numbers which give a
 *          positive answer.
 * https://oeis.org/A328933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, base, dumb, fini, full
 * Formula:   negative integers k such that (digitsum(-k) -
 *            2*firstdigit(-k))^2 > -k. The sequence a(n) lists those k
 *            (negative) in decreasing numeric order (i.e. -2, -3, …). The
 *            sequence is finite; it has 26 terms
 * Range:     i8 n <= 15, i16 n <= 26, i32 n <= 26, i64 n <= 26, i128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t zombies[32];
static size_t zombies_len = 0;

static void init_zombies(void)
{
    if (zombies_len != 0) return;          /* already initialized */
    for (uint64_t m = 2; m <= 2000; ++m) {  /* upper bound large enough for all cases */
        uint64_t sum = 0;
        uint64_t t   = m;
        while (t > 0) {
            sum += t % 10;
            t   /= 10;
        }
        /* first (most significant) digit */
        t = m;
        while (t >= 10) t /= 10;
        int first = (int)t;
        int diff  = (int)sum - 2 * first;
        uint64_t diffSq = (uint64_t)diff * (uint64_t)diff;
        if (diffSq > m) {
            zombies[zombies_len++] = -(seq_t)m;
            if (zombies_len == sizeof(zombies)/sizeof(zombies[0])) break;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */
    init_zombies();
    if (n > zombies_len) return 0;   /* undefined beyond the finite sequence */
    return zombies[n - 1];
}
