/*
 * A329323  Triangle read by rows: T(n,k) is the sum of the parts congruent to
 *          0 mod k in the partitions of n into equal parts, 1 <= k <= n.
 * https://oeis.org/A329323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the concatenation of the triangle T(m,k) where T(m,k) =
 *            m * τ(m/k) if k divides m, otherwise 0. Here τ(x) is the number
 *            of divisors of x. The sequence is read row‑by‑row: a(1)=T(1,1),
 *            a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), …
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* ------------------------------------------------------------------ */
/* helper: divisor count τ(x) for x ≥ 1                                 */
static inline seq_t divisor_count(idx_t x)
{
    seq_t tau = 1;
    idx_t t = x;
    for (idx_t p = 2; (idx_t)p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            idx_t cnt = 0;
            while (t % p == 0)
            {
                t /= p;
                ++cnt;
            }
            tau *= (cnt + 1);
        }
    }
    if (t > 1)            /* remaining prime factor */
        tau *= 2;
    return tau;
}

/* ------------------------------------------------------------------ */
/* Main function required by the build                                 */
seq_t a(idx_t n)
{
    if (n == 0)            /* out of range – not used, return 0 */
        return 0;

    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t low = 1, high = 1;
    while ((__int128)high * (high + 1) / 2 < n)
        high <<= 1;               /* double until high large enough */

    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        __int128 val = (__int128)mid * (mid + 1) / 2;
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number */

    /* k = position inside the row */
    idx_t prev = (r - 1) * r / 2;
    idx_t k = n - prev;

    /* T(r,k): 0 if r not divisible by k */
    if (r % k != 0)
        return 0;

    idx_t mdiv = r / k;                /* the divisor inside τ */
    seq_t tau = divisor_count(mdiv);
    return (seq_t)r * tau;
}
