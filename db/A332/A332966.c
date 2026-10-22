/*
 * A332966  a(n) is the largest value in the sequence s defined by s(1) = 0
 *          and for any k > 0, s(k+1) = (s(k)^2+1) mod n.
 * https://oeis.org/A332966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = maximum value of the sequence defined by s(1) = 0, s(k+1)
 *            = (s(k)^2 + 1) mod n for k ≥ 1 a(1) = 0. The function works for
 *            any n ≥ 1
 * Range:     u8 n <= 261, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   step_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/step_mod.h"
#ifdef __cplusplus

#else
/* In C use a wider type when the compiler supports __int128 to avoid overflow. */
#if defined(__SIZEOF_INT128__)

#else

#endif
#endif

seq_t a(idx_t n)
{
    if (n == 0)   /* safety guard – the sequence is defined only for n ≥ 1. */
        return 0;

    const seq_t mod = (seq_t)n;
    seq_t tortoise = 0, hare = 0, maxv = 0;

    /* Floyd's cycle detection, also keeping track of the maximum value seen. */
    do {
        tortoise = step_mod(tortoise, mod);
        hare     = step_mod(hare, mod);
        hare     = step_mod(hare, mod);
        if (tortoise > maxv) maxv = tortoise;
        if (hare > maxv)     maxv = hare;
    } while (tortoise != hare);

    /* Find the starting point of the cycle. */
    seq_t ptr1 = 0, ptr2 = tortoise;
    while (ptr1 != ptr2) {
        ptr1 = step_mod(ptr1, mod);
        ptr2 = step_mod(ptr2, mod);
    }

    /* Traverse the whole cycle to ensure the maximum is found. */
    seq_t cur = ptr1;
    do {
        if (cur > maxv) maxv = cur;
        cur = step_mod(cur, mod);
    } while (cur != ptr1);

    return maxv;
}
