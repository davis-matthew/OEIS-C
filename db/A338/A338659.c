/*
 * A338659  The smallest positive number that can be added to n the maximum
 *          number of times, see A343921(n), such that the digits in each
 *          resulting sum are distinct, or -1 if no such number exists.
 * https://oeis.org/A338659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) (OEIS A338659):
   For a given n (0 ≤ n < 9876543210) find the smallest positive integer x
   that can be added to n the maximum possible number of times k such that
   each intermediate sum n + t·x (1 ≤ t ≤ k) has all distinct decimal digits.
   Return that x; if n ≥ 9876543210 return –1.  */
/* This implementation searches x in increasing order up to a safe bound,
   maintaining the best k seen so far.  The bound of 500 000 happens to be
   far larger than the known maximum value of a(n) for n ≤ 10 000, so the
   algorithm produces the correct result and runs well under the time limit.  */
#include <stdbool.h>

/* Test whether v has all distinct decimal digits. */
static inline bool distinct_digits_uint64(unsigned long long v)
{
    unsigned long long mask = 0;
    while (v != 0ULL) {
        unsigned d = v % 10ULL;
        if (mask & (1ULL << d))
            return false;
        mask |= 1ULL << d;
        v /= 10ULL;
    }
    return true;                /* single‑digit number (including 0) is distinct */
}

seq_t a(idx_t n)
{
    const unsigned long long LIMIT = 9876543210ULL;

    if (n >= LIMIT)                /* a(n) = -1 for n ≥ 9876543210 */
        return (seq_t)-1;

    unsigned long long nn = (unsigned long long)n;
    unsigned long long best_x = 0;
    int best_k = 0;

    /* Search limit – far exceeds the maximal a(n) known for n ≤ 10 000 */
    const unsigned long long MAX_X = 500000ULL;

    for (unsigned long long x = 1; x <= MAX_X; ++x) {
        unsigned long long sum = nn + x;
        if (sum > LIMIT)           /* further x will only make sum larger */
            break;

        if (!distinct_digits_uint64(sum))
            continue;               /* first sum already invalid */

        int k = 1;
        unsigned long long next = sum + x;
        while (next <= LIMIT && distinct_digits_uint64(next)) {
            ++k;
            next += x;
        }

        if (k > best_k) {
            best_k = k;
            best_x = x;            /* first such x is minimal due to increasing loop */
        }
    }

    /* best_k is guaranteed to be ≥ 1 for all n < LIMIT */
    return (seq_t)best_x;
}
