/*
 * A338712  Numbers with all digits equal and from the set {1, 3, 7, 9}.
 * https://oeis.org/A338712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 38, u64 n <= 77, u128 n <= 154,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A338712 – Numbers with all digits equal and from the set {1,3,7,9}.
   The terms are enumerated in increasing order:
   1, 3, 7, 9, 11, 33, 77, 99, 111, 333, …                    

   Recurrence (valid for n>8):
     a(n) = 11*a(n-4) - 10*a(n-8)

   Initial terms:
     a(1)=1, a(2)=3, a(3)=7, a(4)=9,
     a(5)=11, a(6)=33, a(7)=77, a(8)=99
*/

#include <stdlib.h>     /* malloc, free */

seq_t a(idx_t n)
{
    if (n == 0) /* undefined, but safe sentinel */
        return 0;

    /* Allocate a temporary array large enough for indices 1..n */
    seq_t *term = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!term)
        return 0;   /* allocation failure – unlikely for n≤10000 */

    /* Base values */
    if (n >= 1) term[1] = (seq_t)1;
    if (n >= 2) term[2] = (seq_t)3;
    if (n >= 3) term[3] = (seq_t)7;
    if (n >= 4) term[4] = (seq_t)9;
    if (n >= 5) term[5] = (seq_t)11;
    if (n >= 6) term[6] = (seq_t)33;
    if (n >= 7) term[7] = (seq_t)77;
    if (n >= 8) term[8] = (seq_t)99;

    /* Recurrence for n > 8 */
    for (idx_t i = 9; i <= n; ++i) {
        term[i] = ((seq_t)11 * term[i - 4]) - ((seq_t)10 * term[i - 8]);
    }

    seq_t result = term[n];
    free(term);
    return result;
}
