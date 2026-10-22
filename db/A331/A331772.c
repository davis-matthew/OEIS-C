/*
 * A331772  a(n) = Sum_{-n<i<n, -n<j<n, gcd{i,j}=2} (n-|i|)*(n-|j|).
 * https://oeis.org/A331772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 25, u32 n <= 409, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Factor computation of OEIS sequence A331772
   a(n) = Sum_{-n<i<n, -n<j<n, gcd(i,j)=2} (n-|i|)*(n-|j|)
   The sum is evaluated via a Möbius transform. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;

    idx_t maxk = (n - 1) / 2;          /* maximum |i'| = ⌊(n-1)/2⌋   */

    /* Size limits: for n ≤ 10000     →   maxk ≤ 4999   */
    /* Allocate static arrays of size 5001 (0..5000) for reuse.   */
    static seq_t f[5001];
    static seq_t sumMul[5001];
    static int mu[5001];
    static int primes[5001];
    static bool isComp[5001];
    static int primeCnt = 0;

    /* clear auxiliary arrays for current computation */
    for (idx_t i = 0; i <= maxk; ++i)
        isComp[i] = false;
    primeCnt = 0;

    /* linear sieve for Möbius μ */
    mu[1] = 1;
    for (idx_t i = 2; i <= maxk; ++i)
    {
        if (!isComp[i])
        {
            primes[primeCnt++] = (int)i;
            mu[i] = -1;
        }
        for (int pi = 0; pi < primeCnt; ++pi)
        {
            idx_t prod = i * (idx_t)primes[pi];
            if (prod > maxk)
                break;
            isComp[prod] = true;
            if (i % primes[pi] == 0)
            {
                mu[prod] = 0;
                break;
            }
            else
            {
                mu[prod] = -mu[i];
            }
        }
    }

    /* f[u] = n - 2u  (u = 1 .. maxk) */
    for (idx_t u = 1; u <= maxk; ++u)
        f[u] = (seq_t)n - (seq_t)2 * (seq_t)u;

    /* sumMul[d] = Σ_{k≤maxk, d|k} f[k]   */
    for (idx_t d = 1; d <= maxk; ++d)
        sumMul[d] = 0;
    for (idx_t d = 1; d <= maxk; ++d)
        for (idx_t k = d; k <= maxk; k += d)
            sumMul[d] += f[k];

    /* Compute S = Σ_{d=1..maxk} μ(d) * (sumMul[d])²  */
    seq_t posSum = 0, negSum = 0;
    for (idx_t d = 1; d <= maxk; ++d)
    {
        if (mu[d] == 0)
            continue;
        seq_t v = sumMul[d];
        seq_t sq = v * v;
        if (mu[d] == 1)
            posSum += sq;
        else /* mu[d] == -1 */
            negSum += sq;
    }
    seq_t S = posSum - negSum;          /* posSum ≥ negSum for all n */

    /* add contributions from pairs involving zero rows/columns */
    seq_t total = (seq_t)4 * (seq_t)n * ((seq_t)n - 2) + (seq_t)4 * S;
    return total;
}
