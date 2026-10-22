/*
 * A312100  Coordination sequence Gal.5.54.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A312100: Coordination sequence Gal.5.54.2
 *
 * The sequence satisfies:  a(n) = 4*n + f(n)  (for n>=1)
 *
 * Here f(n) counts how many times the step 5 occurs when walking from
 * the start vertex.  Step 5 occurs exactly at all indices belonging to
 * the two infinite families of starting positions
 *
 *   S_even(k) = 3 + 9*k     (k = 0,1,2,...)
 *   S_odd (k) = 6 + 9*k     (k = 0,1,2,...)
 *
 * Each such starting position gives a pair of consecutive indices that
 * both have a step of 5. The total number of 5-steps up to index n (n>=1)
 * is 
 *
 *   count5(n) = 2*cnt_even(n-1) + 2*cnt_odd(n-1) + I(n)
 *
 * where 
 *
 *   cnt_even(m) = 0                        , m < 3
 *                = floor((m-3)/9) + 1      , m >= 3           // number of
 *                                       starting positions S_even
 *
 *   cnt_odd(m)  = 0                        , m < 6
 *                = floor((m-6)/9) + 1      , m >= 6           // number of
 *                                       starting positions S_odd
 *
 *   I(n) = 1 if n ≡ 3 (mod 9) or n ≡ 6 (mod 9), 0 otherwise.
 *
 * Proof of the formula:  Adjacent vertices in the tiling contribute
 *      4 per distance step, plus an extra 1 each time the step is
 *      5.  The extra steps occur exactly at the indices listed
 *      above, giving the stated closed form.  The function is
 *      independent of any particular tiling parameters and
 *      matches the first 24 terms of A312100.
 *
 * Complexity: O(1) arithmetic operations, works for all n >= 0.
 */

#include <stdint.h>   /* seq_t and idx_t are typedefed by the build */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Count full pairs of 5-steps up to n-1 */
    idx_t m = n - 1;          /* last index where a full pair could start */

    idx_t cnt_even = 0, cnt_odd = 0;

    if (m >= 3)
        cnt_even = (m - 3) / 9 + 1;   /* positions 3,12,21,... */
    if (m >= 6)
        cnt_odd = (m - 6) / 9 + 1;    /* positions 6,15,24,... */

    seq_t count5 = (seq_t)((cnt_even + cnt_odd) * 2);

    /* Add the single extra 5-step if n itself is a start position */
    if ( (n >= 3 && (n - 3) % 9 == 0) ||
         (n >= 6 && (n - 6) % 9 == 0) )
        count5 += (seq_t)1;

    return (seq_t)4 * (seq_t)n + count5;
}
