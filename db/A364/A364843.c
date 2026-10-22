/*
 * A364843  Integers are repeated in runs of 1, 2, 3, ... Each new integer
 *          (following a run) is given the value of its sequence index value.
 * https://oeis.org/A364843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   Let k be the smallest integer with k(k+1)/2 ≥ n. * Then a(n) = 1
 *            + (k-1)k/2 (the value of the integer in the run that * contains
 *            position n). This implements a binary search for k and *
 *            computes the run value directly. No table lookup is used. The *
 *            function works for any n ≥ 1 and for any seq_t...
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* binary search for k: k(k+1)/2 >= n */
    idx_t low  = 1;
    idx_t high = n;           /* k can never exceed n */
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        // use 128‑bit arithmetic to avoid overflow of mid*(mid+1)/2
        unsigned __int128 prod = (unsigned __int128)mid * (mid + 1) / 2;
        if (prod >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t k = low; /* run index */

    /* compute the run value: 1 + (k-1)k/2 */
    unsigned __int128 val128 = (unsigned __int128)(k - 1) * k / 2 + 1;
    return (seq_t)val128;
}
