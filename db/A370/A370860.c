/*
 * A370860  Numbers m such that c(0) <= c(1) > c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370860
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute‑force search over integers, counting distinct
   numbers whose ternary digits satisfy
   count(0) <= count(1) > count(2). */

#include <stdbool.h>

/* Helper: test a single number for the property. */
static bool good(seq_t m)
{
    int cnt[3] = {0, 0, 0};
    seq_t x = m;
    while (x > 0) {
        int d = (int)(x % 3);
        cnt[d]++;
        x /= 3;
    }
    return cnt[0] <= cnt[1] && cnt[1] > cnt[2];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined, return 0 */

    idx_t seen = 0;
    seq_t m = 1;                 /* start from the smallest positive integer */

    while (seen < n) {
        if (good(m)) {
            seen++;
            if (seen == n)
                return m;
        }
        m += 1;
    }
    return m;   /* unreachable, but keeps the compiler happy */
}
