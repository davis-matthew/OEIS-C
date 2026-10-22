/*
 * A328292  a(n) is the greatest nonnegative multiple of n with distinct
 *          decimal digits.
 * https://oeis.org/A328292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the greatest multiple of n (≤ 9876543210) whose decimal
   digits are all distinct.  This implements OEIS A328292. */
#include <stdbool.h>

/* Check whether x has all distinct decimal digits. */
static bool distinct_digits(seq_t x) {
    unsigned mask = 0;
    if (x == 0)  /* 0 has one distinct digit (0) */
        return true;
    while (x) {
        unsigned d = (unsigned)(x % 10);
        if (mask & (1u << d))
            return false;          /* duplicate digit */
        mask |= 1u << d;
        x /= 10;
    }
    return true;
}

seq_t a(idx_t n) {
    if (n == 0)          /* defined only for n ≥ 1, but safe anyway */
        return 0;

    /* maximal possible value with distinct digits for any n > 0 */
    const seq_t LIMIT = (seq_t)9876543210ULL;

    /* If n exceeds the limit, the only candidate is 0 */
    if (n > (idx_t)LIMIT)
        return 0;

    seq_t ncast = (seq_t)n;
    seq_t m = (LIMIT / ncast) * ncast;   /* largest multiple of n ≤ LIMIT */

    for (seq_t cur = m; ; ) {
        if (distinct_digits(cur))
            return cur;               /* greatest valid multiple found */
        if (cur < ncast)             /* cannot go lower without reaching 0 */
            break;
        cur -= ncast;
    }
    /* The loop terminates only when cur == 0 (since 0 is always valid) */
    return 0;
}
