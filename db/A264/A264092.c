/*
 * A264092  Number of (3+1)X(n+1) arrays of permutations of 0..n*4+3 with each
 *          element having index change +-(.,.) 0,0 0,1 or 2,-2.
 * https://oeis.org/A264092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A264092
    Formula:  a(n) = 14*a(n-1) + 14*a(n-2) - 210*a(n-3) + 210*a(n-5)
                - 14*a(n-6) - 14*a(n-7) + a(n-8)
    Offset: 1
    Initial values:
        a(1)=16, a(2)=169, a(3)=2401, a(4)=32761, a(5)=456976,
        a(6)=6355441, a(7)=88529281, a(8)=1232922769
    The function may be called with n in any order; it memoises values
    and requires O(1) time per new n after initial growth.
*/

#include <stdlib.h>   /* malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Zero or negative indices are outside the offset – return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache:  memo[ i ] holds a(i)  for 1 ≤ i ≤ last.
       memo[0] is unused.                                                       */
    static seq_t *memo      = NULL;
    static size_t memocap   = 0;   /* number of allocated slots (≥ last+1) */
    static idx_t  last      = 0;   /* highest index computed so far       */

    /* Grow the allocation if necessary. */
    if ((size_t)(n + 1) > memocap) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (memo == NULL) ?
            (seq_t *)malloc(sizeof(seq_t) * newcap) :
            (seq_t *)realloc(memo, sizeof(seq_t) * newcap);
        if (tmp == NULL) /* allocation failed – abort, as per requirement. */
            abort();
        memo   = tmp;
        memocap = newcap;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last + 1; i <= n; ++i) {
        switch (i) {
        case 1: memo[1] = (seq_t)16;              break;
        case 2: memo[2] = (seq_t)169;             break;
        case 3: memo[3] = (seq_t)2401;            break;
        case 4: memo[4] = (seq_t)32761;           break;
        case 5: memo[5] = (seq_t)456976;          break;
        case 6: memo[6] = (seq_t)6355441;         break;
        case 7: memo[7] = (seq_t)88529281;        break;
        case 8: memo[8] = (seq_t)1232922769;      break;
        default:
            /* All indices above 8 use the recurrence. */
            seq_t term1 = (seq_t)14 * memo[(size_t)(i - 1)];
            seq_t term2 = (seq_t)14 * memo[(size_t)(i - 2)];
            seq_t term3 = (seq_t)210 * memo[(size_t)(i - 3)];
            seq_t term5 = (seq_t)210 * memo[(size_t)(i - 5)];
            seq_t term6 = (seq_t)14  * memo[(size_t)(i - 6)];
            seq_t term7 = (seq_t)14  * memo[(size_t)(i - 7)];
            seq_t term8 =              memo[(size_t)(i - 8)];
            memo[i] = term1 + term2 - term3 + term5 - term6 - term7 + term8;
        }
    }

    last = n;
    return memo[(size_t)n];
}
