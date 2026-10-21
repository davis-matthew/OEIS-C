/*
 * A274801  Number of n X 6 0..2 arrays with no element equal to any value at
 *          offset (-1,-2) (-2,-1) (0,-1) or (-1,0) and new values introduced
 *          in order 0..2.
 * https://oeis.org/A274801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A274801: Number of n×6 arrays with entries 0..2 avoiding
   offsets (-1,-2), (-2,-1), (0,-1), (-1,0) and introducing new
   values in order 0..2.
   Recurrence (for n>15):
     a(n) = 2*a(n-1) + 80*a(n-2) + 140*a(n-3) - 474*a(n-4) - 998*a(n-5)
            - 130*a(n-6) + 781*a(n-7) + 814*a(n-8)
            - 79*a(n-9) - 360*a(n-10) + 54*a(n-11) + 48*a(n-12)
            - 52*a(n-13) + 4*a(n-14)
   Initial values:
     a(1)=16, a(2)=164, a(3)=1572, a(4)=16328, a(5)=169328,
     a(6)=1774372, a(7)=18476064, a(8)=193292660,
     a(9)=2016460140, a(10)=21072173792, a(11)=219985654388,
     a(12)=2297904486000, a(13)=23995177354892,
     a(14)=250610794543168, a(15)=2617142836973944.
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static const seq_t init[15] = {
    (seq_t)16ULL,
    (seq_t)164ULL,
    (seq_t)1572ULL,
    (seq_t)16328ULL,
    (seq_t)169328ULL,
    (seq_t)1774372ULL,
    (seq_t)18476064ULL,
    (seq_t)193292660ULL,
    (seq_t)2016460140ULL,
    (seq_t)21072173792ULL,
    (seq_t)219985654388ULL,
    (seq_t)2297904486000ULL,
    (seq_t)23995177354892ULL,
    (seq_t)250610794543168ULL,
    (seq_t)2617142836973944ULL
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n <= 15u) return init[n - 1];

    /* allocate array for terms up to n (0‑based index: a(1) at 0) */
    seq_t *terms = alloc_seq_array((size_t)n);
    if (!terms) return 0;  /* allocation failed */

    /* copy initial values */
    for (idx_t i = 0; i < 15u; ++i) terms[i] = init[i];

    /* compute further terms using recurrence */
    for (idx_t k = 15u; k < n; ++k) {
        seq_t term =
            (seq_t)2 * terms[k - 1] +
            (seq_t)80 * terms[k - 2] +
            (seq_t)140 * terms[k - 3] -
            (seq_t)474 * terms[k - 4] -
            (seq_t)998 * terms[k - 5] -
            (seq_t)130 * terms[k - 6] +
            (seq_t)781 * terms[k - 7] +
            (seq_t)814 * terms[k - 8] -
            (seq_t)79 * terms[k - 9] -
            (seq_t)360 * terms[k - 10] +
            (seq_t)54 * terms[k - 11] +
            (seq_t)48 * terms[k - 12] -
            (seq_t)52 * terms[k - 13] +
            (seq_t)4 * terms[k - 14];
        terms[k] = term;
    }

    seq_t result = terms[n - 1];
    free_seq_array(terms);
    return result;
}
