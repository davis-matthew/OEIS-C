/*
 * A397125  a(n) = smallest happy number h such that n*h is happy.
 * https://oeis.org/A397125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest happy h such that n*h is happy
 * Range:     u8 n <= 2, u16 n <= 3236, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/* Return true iff x is a happy number in base 10. */
static bool is_happy(seq_t x)
{
    /* The sum of squares of decimal digits of any integer <= 81 * number of digits.
       For even very large numbers this is well below 20000. */
    enum { MAX_SUM = 20000 };
    static bool seen[MAX_SUM];
    memset(seen, 0, sizeof(seen));

    while (1) {
        /* Compute next value as sum of squares of digits. */
        unsigned int sum = 0;
        seq_t tmp = x;
        while (tmp > 0) {
            unsigned int d = (unsigned int)(tmp % 10);
            sum += d * d;
            tmp /= 10;
        }
        if (sum == 1) return true;
        if (sum >= MAX_SUM) return false; /* should not happen */
        if (seen[sum]) return false;
        seen[sum] = true;
        x = (seq_t)sum;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    if (is_happy(N)) return 1;

    seq_t h = 2;
    for (;; ++h) {
        if (!is_happy(h)) continue;
        if (is_happy(N * h)) return h;
    }
}
