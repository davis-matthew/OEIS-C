/*
 * A392969  Positive integers whose multiset of digits cannot be reduced to
 *          {0} by repeatedly replacing two digits with the digits of their
 *          sum, absolute difference, or product.
 * https://oeis.org/A392969
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   reducible_state
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes terms of OEIS A392969.
   The algorithm generates all numbers up to 98 and tests each for
   reducibility.  A number is reducible if its digit multiset can
   be reduced to {0} by repeatedly replacing two digits by the digits
   of their sum, absolute difference or product.  Since the sequence
   is finite and 98 is the largest term, this exhaustive search
   suffices.  The result is memoized in a static array.  The code
   uses only integer arithmetic and standard C11 constructs. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/reducible_state.h"

/* Recursive test whether a multiset of decimal digits can reduce
   to {0}.  This unbounded DFS explores all possible choices of
   two digits and all three operations.  The maximum number of
   digits encountered is well below 12, so fixed-size arrays are
   sufficient. */

/* Determine whether an integer n (1 ≤ n ≤ 98) is reducible. */
static bool is_reducible(int n) {
    int digits[12], len = 0;
    int temp = n;
    if (temp == 0)
        digits[len++] = 0;
    else
        while (temp) { digits[len++] = temp % 10; temp /= 10; }

    return reducible_state(digits, len);
}

/* Main sequence generator with memoization. */
seq_t a(idx_t n) {
    static seq_t seq[150];
    static idx_t seq_len = 0;
    static bool initialized = false;

    if (!initialized) {
        seq_len = 0;
        /* The sequence contains all numbers up to 98 that are not reducible. */
        for (int num = 1; num <= 98; ++num) {
            if (!is_reducible(num))
                seq[seq_len++] = (seq_t)num;
        }
        initialized = true;
    }

    if (n == 0 || n > seq_len) return (seq_t)0;
    return seq[n - 1];
}
