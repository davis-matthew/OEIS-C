/*
 * A336823  a(1)=1; thereafter a(n) = a(n-1) / wt(n-1) if wt(n-1) divides
 *          a(n-1), otherwise a(n) = a(n-1) * wt(n-1) where wt(n) is the
 *          binary weight of n.
 * https://oeis.org/A336823
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, look
 * Formula:   a(1)=1; for n>1, a(n)=a(n-1)/wt(n-1) if wt(n-1) divides a(n-1),
 *            * otherwise a(n)=a(n-1)*wt(n-1); wt(m)=A000120(m) (binary weight
 *            of m)
 * Range:     u8 n <= 135, u16 n <= 2551, u32 n <= 9982, u64 n <= 9982,
 *            u128 n <= 9982, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return the number of set bits in a 64‑bit integer. */
static inline unsigned int popcount_u64(idx_t x)
{
    return __builtin_popcountll((unsigned long long)x);
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined, but return 0 for safety */

    seq_t curr = (seq_t)1;      /* a(1) */
    if (n == 1)
        return curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        idx_t prev_index = i - 1;          /* weight of n-1 uses index i-1 */
        unsigned int w = popcount_u64(prev_index);
        if (w == 0) w = 1;                 /* safety, w is never 0 for i>1 */
        if (curr % (seq_t)w == 0)
            curr = curr / (seq_t)w;
        else
            curr = curr * (seq_t)w;
    }
    return curr;
}
