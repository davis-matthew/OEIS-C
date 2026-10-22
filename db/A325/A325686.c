/*
 * A325686  Number of strict length-3 compositions x + y + z = n satisfying x
 *          + y != z and x != y + z.
 * https://oeis.org/A325686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 366, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A325686: Number of strict length‑3 compositions x+y+z = n
   with x + y != z and x != y + z.
   Formula used:  a(n) = distinctTriples(n) – 2·count(x+y = z),
   with    
     distinctTriples(n) = (n-1)(n-2)/2 – 3·⌊(n-1)/2⌋ + 2·[n mod 3 = 0],
     count(x+y = z)  = 
       0                               if n is odd,
       (k-1) – [k even]                if n is even,  n = 2k .
   The 3‑term forbidden pattern y+z = x yields the same count,
   hence the factor 2.
   This implementation is O(1) time and uses only integer
   arithmetic.  For n < 6 the answer is 0, matching the
   OEIS offset.
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < 6) return 0;

    seq_t NN = (seq_t)n;
    /* total ordered positive triples summing to n */
    seq_t total = (NN - 1) * (NN - 2) / 2;

    seq_t mm1 = (NN - 1) / 2;                /* floor((n-1)/2)   */
    seq_t all_equal = (n % 3 == 0) ? 1 : 0;   /* 1 if n divisible by 3 */

    /* distinct triples (all parts distinct) */
    seq_t distinct = total - 3 * mm1 + 2 * all_equal;

    /* count of triples satisfying x + y = z */
    seq_t forbid_per = 0;
    if ((n & 1) == 0) {                      /* n even */
        seq_t k = NN / 2;                    /* n = 2k   */
        if ((k & 1) == 0)                   /* k even  */
            forbid_per = k - 2;
        else                                 /* k odd   */
            forbid_per = k - 1;
    }

    seq_t forbidden = 2 * forbid_per;         /* two symmetric cases */

    return distinct - forbidden;
}
