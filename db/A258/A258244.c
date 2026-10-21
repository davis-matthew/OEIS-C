/*
 * A258244  Number of steps from n to 0, where allowable steps are x -> [x/e]
 *          if x = is in A022843 (the Beatty sequence for e) and x -> [e*x]
 *          otherwise, where [ ] = floor.
 * https://oeis.org/A258244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of edges from 0 to n in the tree where each step
 *            follows: if x ∈ {⌊k·e⌋} (Beatty for e) → x ← ⌊x/e⌋ otherwise → x
 *            ← ⌊e·x⌋ a(0) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static const long double e = 2.718281828459045235360287471352662497756911591;
static inline bool in_beatty(idx_t n)
{
    long double f1 = (long double)n / e;
    long double f2 = (long double)(n + 1) / e;
    return ((idx_t)f1 != (idx_t)f2);
}

seq_t a(idx_t n)
{
    idx_t cur = n;
    seq_t steps = 0;
    while (cur > 0) {
        if (in_beatty(cur))
            cur = (idx_t)((long double)cur / e);      /* floor(x/e) */
        else
            cur = (idx_t)((long double)cur * e);      /* floor(e*x) */
        steps++;
    }
    return steps;
}
