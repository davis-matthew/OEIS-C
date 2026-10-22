/*
 * A368876  a(n) is the number of ways n can be reached by the following
 *          method: we start with 1, then add or multiply alternately, and
 *          each operand must be 3 or 4 or 5.
 * https://oeis.org/A368876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A368876: number of ways to reach n by alternating + or × by 3,4,5
   starting from 1.  The sequence satisfies the recurrence:
     a(n) = #{paths 1 → n where operations alternate between + and ×,
               using only values 3,4,5.
   Implementation: dynamic programming with two tables
   add[v], mul[v] – number of paths ending with a + or ×, respectively.
*/

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fast path for the offset */
    if (n == 1)
        return (seq_t)1;

    /* Static state for memoization */
    static idx_t max_n = 0;
    static seq_t *add = NULL;
    static seq_t *mul = NULL;
    static size_t cap = 0;      /* capacity of arrays */

    /* Ensure capacity for index n */
    if (n + 1 > cap) {
        size_t new_cap = (n + 1) * 2;
        if (new_cap < 2) new_cap = 2;
        seq_t *new_add = (seq_t *)realloc(add, new_cap * sizeof(seq_t));
        seq_t *new_mul = (seq_t *)realloc(mul, new_cap * sizeof(seq_t));
        if (!new_add || !new_mul) /* allocation failure: abort */
            return (seq_t)0;
        add = new_add;
        mul = new_mul;
        /* Zero initialise the newly allocated region */
        for (size_t i = cap; i < new_cap; ++i) {
            add[i] = 0;
            mul[i] = 0;
        }
        cap = new_cap;
    }

    /* Initialise base case at first use */
    if (max_n == 0) {
        /* We keep index 0 unused.  add[1] and mul[1] are set to 1
           so that a first operation (add or mul) from 1 is counted. */
        add[1] = (seq_t)1;
        mul[1] = (seq_t)1;
        max_n  = 1;
    }

    /* Extend DP tables up to n */
    for (idx_t v = max_n + 1; v <= n; ++v) {
        seq_t add_v = 0;
        seq_t mul_v = 0;

        /* Contributions to add[v] from a previous multiplication */
        if (v > 3)
            add_v += mul[v - 3];
        if (v > 4)
            add_v += mul[v - 4];
        if (v > 5)
            add_v += mul[v - 5];

        /* Contributions to mul[v] from a previous addition */
        if (v % 3 == 0)
            mul_v += add[v / 3];
        if (v % 4 == 0)
            mul_v += add[v / 4];
        if (v % 5 == 0)
            mul_v += add[v / 5];

        add[v] = add_v;
        mul[v] = mul_v;
    }
    max_n = n;

    return add[n] + mul[n];
}
