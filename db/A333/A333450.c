/*
 * A333450  a(n) = Sum_{k=1..n} mu(k) * prime(floor(n/k)).
 * https://oeis.org/A333450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} μ(k) * prime(floor(n/k))
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   get_prime, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;      /* undefined for n=0, but safe */

    seq_t pos = 0;            /* sum of terms with μ(k)=+1 */
    seq_t neg = 0;            /* sum of terms with μ(k)=-1 */

    for (idx_t k = 1; k <= n; ++k) {
        int mu = mu_of(k);            /* μ(k) ∈ {–1,0,1} */
        if (mu == 0) continue;        /* skip multiples with μ(k)=0 */

        /* prime(floor(n/k)) – the k-th element in the prime sequence. */
        seq_t p = (seq_t)get_prime(n / k);

        if (mu > 0)  pos += p;
        else         neg += p;         /* mu == -1 */
    }

    /* The result is guaranteed to be non‑negative. */
    return pos - neg;
}
