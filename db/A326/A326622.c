/*
 * A326622  Number of factorizations of n into factors > 1 with integer
 *          average.
 * https://oeis.org/A326622
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of factorizations of n into factors > 1 with
 *            integer average
 * Range:     u8 n <= 2875, u16 n <= 9636, u32 n <= 9636, u64 n <= 9636,
 *            u128 n <= 9636, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Recursive helper that counts valid factorizations.
   rem   : the remaining value to factorize
   start : the smallest factor allowed in the current position (to avoid duplicates)
   sum   : the sum of factors chosen so far
   k     : the number of factors chosen so far
   cnt   : pointer to the running count
*/
static void
count_rec(idx_t rem, idx_t start, seq_t sum, seq_t k, seq_t *cnt)
{
    if (rem == 1) {
        if (k > 0 && (sum % k) == 0)
            (*cnt)++;
        return;
    }
    for (idx_t f = start; f <= rem; ++f) {
        if (rem % f == 0)
            count_rec(rem / f, f, sum + f, k + 1, cnt);
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0, a(0) irrelevant */
    seq_t count = 0;
    count_rec(n, 2, 0, 0, &count);
    return count;
}
