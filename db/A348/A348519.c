/*
 * A348519  Tetraphobe or 4-phobe numbers: integers that are not tetraphile
 *          numbers.
 * https://oeis.org/A348519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 23, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Validated: full, pass, 23 terms through n = 23 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A348519: 4-phobe numbers (elements not representable as a sum of four
   increasing positive integers b1<b2<b3<b4 with b1|b2, b2|b3, b3|b4).
   The set is finite; it contains exactly 23 numbers, the largest being 48.
   This implementation enumerates all possible chains up to the known
   maximum and builds the sequence lazily.  The search is inexpensive
   because the largest tetraphobe is 48. */

#include <stdint.h>
#include <stdbool.h>

/* Return true iff x can be written as a sum of a divisibility chain
   of four increasing integers b1<b2<b3<b4.  The routine is safe for
   all x that fit in seq_t; it handles the trivial cases x<10 early
   to avoid underflow in the loop bounds. */
static bool is_tetraphile(seq_t x)
{
    /* For very small values the chain is impossible */
    if (x < 10) return false;

    idx_t n = (idx_t)x;          /* safe because x <= 48 in this family */
    /* b1 must be at least 1 and at most n-6 (to leave room for the other
       three strictly increasing numbers).  The cast is safe as n <= 48. */
    if (n < 7) return false;     /* guard against underflow in n-6 */

    for (idx_t b1 = 1; b1 <= n - 6; ++b1) {
        /* b2 = b1 * k2, k2 >= 2 */
        for (idx_t k2 = 2; ; ++k2) {
            idx_t b2 = b1 * k2;
            if (b2 > n - 3) break;         /* must leave room for b3,b4 */
            for (idx_t k3 = 2; ; ++k3) {
                idx_t b3 = b2 * k3;
                if (b3 > n - 2) break;
                for (idx_t k4 = 2; ; ++k4) {
                    idx_t b4 = b3 * k4;
                    if (b4 > n - 1) break;   /* must still sum to n */
                    if (b1 + b2 + b3 + b4 == n) return true;
                }
            }
        }
    }
    return false;
}

/* Main sequence function.  Builds the first 23 terms on the first call.
   Returns 0 for indices beyond the known finite set. */
seq_t a(idx_t n)
{
    /* The total number of tetraphobe numbers is known to be 23. */
    enum { COUNT = 23 };
    static bool built = false;
    static seq_t seq[COUNT];

    if (!built) {
        idx_t found = 0;
        idx_t candidate = 1;
        while (found < COUNT) {
            /* A tetraphobe is a number that is NOT tetraphile */
            if (!is_tetraphile((seq_t)candidate)) {
                seq[found++] = (seq_t)candidate;
            }
            ++candidate;
        }
        built = true;
    }

    if (n >= 1 && n <= COUNT) {
        return seq[n - 1];
    }
    /* For n beyond the finite list, the sequence is undefined. */
    return (seq_t)0;
}
