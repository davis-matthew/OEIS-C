/*
 * A392675  a(n) = Sum_{k=0..floor(n/3)} binomial(k+1,3*n-9*k+1).
 * https://oeis.org/A392675
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} binomial(k+1, 3*n-9*k+1)
 *            Recurrence: a(n) = 3*a(n-3) - 3*a(n-6) + a(n-9) + a(n-10)
 *            Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=2, a(4)=0, a(5)=0,
 *            a(6)=3, a(7)=0, a(8)=0, a(9)=4
 * Range:     u8 n <= 30, u16 n <= 56, u32 n <= 107, u64 n <= 209,
 *            u128 n <= 411, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Base values for n = 0..9 */
    static const seq_t base[10] = {
        (seq_t)1,   /* a(0) */
        (seq_t)0,   /* a(1) */
        (seq_t)0,   /* a(2) */
        (seq_t)2,   /* a(3) */
        (seq_t)0,   /* a(4) */
        (seq_t)0,   /* a(5) */
        (seq_t)3,   /* a(6) */
        (seq_t)0,   /* a(7) */
        (seq_t)0,   /* a(8) */
        (seq_t)4    /* a(9) */
    };

    /* Allocate an array of size n+1.  The helper function takes care of
       construction (for arbitrary‑precision types) and deallocation.    */
    seq_t *arr = alloc_seq_array((size_t)(n + 1));
    if (!arr)
        return (seq_t)0; /* allocation failure */

    /* Fill the base cases up to min(9,n) */
    idx_t upper = n < 9 ? n : 9;
    for (idx_t i = 0; i <= upper; ++i)
        arr[i] = base[i];

    /* Compute the remaining terms using the recurrence.  For
       n >= 10 the recurrence is well defined.                 */
    if (n >= 10) {
        for (idx_t i = 10; i <= n; ++i) {
            arr[i] = ((seq_t)3) * arr[i - 3]
                   - ((seq_t)3) * arr[i - 6]
                   + arr[i - 9]
                   + arr[i - 10];
        }
    }

    seq_t result = arr[n];
    free_seq_array(arr);
    return result;
}
