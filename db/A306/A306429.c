/*
 * A306429  According to Larry Baggett, this sequence appeared on a Florida
 *          College Prep Exam. The exam question was, of course, given six or
 *          seven terms, to find the next term.
 * https://oeis.org/A306429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A306429: a(1) = 6; for n ≥ 2,
   let k = (n-2) mod 7
   a(n) = a(n-1) * 7   if k==0
   a(n) = a(n-1) / 6   if k==1
   a(n) = a(n-1) + 5   if k==2
   a(n) = a(n-1) * 4   if k==3
   a(n) = a(n-1) / 3   if k==4
   a(n) = a(n-1) + 2   if k==5
   a(n) = a(n-1) * 1   if k==6
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined by OEIS; safe fallback */

    seq_t cur = 6;              /* a(1) */

    if (n == 1)
        return cur;

    for (idx_t step = 1; step < n; ++step) {
        /* (step-1) runs 0,1,...,n-2 */
        unsigned op = (unsigned)((step - 1) % 7);
        switch (op) {
            case 0: cur *= 7; break;   /* *7   */
            case 1: cur /= 6; break;   /* /6   */
            case 2: cur += 5; break;   /* +5   */
            case 3: cur *= 4; break;   /* *4   */
            case 4: cur /= 3; break;   /* /3   */
            case 5: cur += 2; break;   /* +2   */
            case 6: cur *= 1; break;   /* *1   */
        }
    }
    return cur;
}
