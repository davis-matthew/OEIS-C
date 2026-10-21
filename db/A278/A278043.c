/*
 * A278043  Number of 1's in tribonacci representation of n (cf. A278038).
 * https://oeis.org/A278043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A000120(A003726(n+1))
 * Range:     u8 n <= 9594, u16 n <= 9594, u32 n <= 9594, u64 n <= 9594,
 *            u128 n <= 9594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Interpreted as the number of tribonacci numbers needed in the greedy
 * decomposition of n (the number of 1's in the tribonacci representation). */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Tribonacci numbers: T0=1, T1=1, T2=2, T3=4, ... */
    /* For 64‑bit n the list is shorter than 70 elements. */
    idx_t trib[70];
    size_t len = 0;

    trib[0] = 1;
    trib[1] = 1;
    trib[2] = 2;
    len      = 3;

    /* Build list until the next tribonacci would exceed n.  Use a wider
     * integer type to avoid overflow during addition. */
    for (;;)
    {
        if (len < 3)
            break;          /* safety, should never happen */

        unsigned __int128 sum = (unsigned __int128)trib[len-1] +
                               (unsigned __int128)trib[len-2] +
                               (unsigned __int128)trib[len-3];

        if (sum > (unsigned __int128)n)
            break;

        if (len >= sizeof(trib)/sizeof(trib[0]))
            break;          /* safety guard */

        trib[len] = (idx_t)sum;
        ++len;
    }

    /* Greedy decomposition counting the used tribonacci numbers. */
    idx_t rem   = n;
    seq_t count = 0;

    for (long long i = (long long)len - 1; i >= 0; --i)
    {
        if (trib[(size_t)i] <= rem)
        {
            rem   -= trib[(size_t)i];
            count += 1;
        }
    }

    return count;
}
