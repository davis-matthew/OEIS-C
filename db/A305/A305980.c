/*
 * A305980  Filter sequence for a(Squarefree numbers > 1) = constant
 *          sequences.
 * https://oeis.org/A305980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n is squarefree; * a(n) = 2 + (n -
 *            count_squarefree(n)) * where count_squarefree(n) =
 *            sum_{d=1}^{⌊√n⌋} μ(d) ⌊n/d²⌋ * and μ(d) is the Möbius function.
 *            * This implements the OEIS definition of A305980
 * Range:     u8 n <= 649, u16 n <= 9605, u32 n <= 9605, u64 n <= 9605,
 *            u128 n <= 9605, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute the Möbius function μ(n). */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int distinct = 0;
    idx_t t = n;
    for (idx_t p = 2; (unsigned __int128)p * p <= t; ++p)
    {
        if (t % p == 0)
        {
            int exp = 0;
            while (t % p == 0)
            {
                t /= p;
                ++exp;
                if (exp > 1) return 0;   /* squared prime factor */
            }
            ++distinct;
        }
    }
    if (t > 1) ++distinct;               /* remaining prime factor */
    return (distinct % 2 == 0) ? 1 : -1;
}

/* Count the number of square‑free integers ≤ n. */
static seq_t squarefree_count(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; ; ++d)
    {
        unsigned __int128 d_sq128 = (unsigned __int128)d * d;
        if (d_sq128 > n) break;
        idx_t d_sq = (idx_t)d_sq128;          /* safe: d² ≤ n ≤ 2^64−1 */
        int mu = mobius(d);
        idx_t term = n / d_sq;
        sum += (seq_t)mu * (seq_t)term;
    }
    return sum;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 1) return 1;                  /* offset 1 */
    int mu_n = mobius(n);
    if (mu_n != 0)                          /* square‑free */
        return 2;
    else {
        seq_t sf = squarefree_count(n);
        seq_t res = 2 + (seq_t)n - sf;
        return res;
    }
}
