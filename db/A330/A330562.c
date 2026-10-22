/*
 * A330562  Positive numbers k with property that if d is any nonzero digit of
 *          k then k mod d is also a digit of k.
 * https://oeis.org/A330562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 7581, u32 n <= 9293, u64 n <= 9293,
 *            u128 n <= 9293, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A330562:
   a(n) = the nth positive integer k for which
          every non‑zero decimal digit d of k satisfies
          k % d is a decimal digit of k.
   (All such k must contain at least one zero digit.)
   The code finds terms by testing integers in increasing order. */

#include <stdbool.h>

/* Helper: true if all required digit conditions hold for k */
static bool valid(seq_t k)
{
    bool present[10] = {0};
    seq_t t = k;
    bool has_zero = false;

    /* Gather digit counts and check for zero */
    while (t > 0) {
        unsigned int d = (unsigned int)(t % 10);
        present[d] = true;
        if (d == 0) has_zero = true;
        t /= 10;
    }
    if (!has_zero) return false;

    /* For every non‑zero digit, check remainder property */
    for (int d = 1; d <= 9; ++d) {
        if (present[d]) {
            seq_t r = k % d;            /* 0 <= r < d <= 9 */
            if (!present[(unsigned int)r]) return false;
        }
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */
    idx_t count = 0;
    idx_t current = 1;               /* smallest positive integer */

    while (1) {
        if (valid((seq_t)current)) {
            ++count;
            if (count == n) return (seq_t)current;
        }
        ++current;
    }
}
