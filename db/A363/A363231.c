/*
 * A363231  Number of partitions of n with rank 4 or higher (the rank of a
 *          partition is the largest part minus the number of parts).
 * https://oeis.org/A363231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1} (-1)^(k-1) * p(n - k*(3*k+7)/2) where p(m) is the
 *            partition function (p(0)=1)
 * Range:     u8 n <= 21, u16 n <= 49, u32 n <= 136, u64 n <= 429,
 *            u128 n <= 1481, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *P = NULL;      /* cache for partition numbers p[0 .. N] */
static size_t Psize = 0;     /* current size of P (number of entries) */

static void ensure_partitions(idx_t N)
/* Ensure that p[0..N] are computed and stored in P. */
{
    if (Psize > 0 && Psize-1 >= N)
        return;          /* already computed up to N */

    /* extend the cache to hold indices up to N */
    size_t newsize = (size_t)(N+1);
    seq_t *tmp = (seq_t*)realloc(P, newsize * sizeof(seq_t));
    if (!tmp) exit(1);
    P = tmp;

    if (Psize == 0) {        /* initialize base case p[0] = 1 */
        P[0] = 1;
        Psize = 1;
    }

    /* compute partition numbers for previously missing indices */
    for (idx_t m = (idx_t)Psize; m <= N; ++m) {
        seq_t total = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t pent1 = k*(3*k - 1)/2;
            if (pent1 > m) break;
            seq_t term = P[m - pent1];
            if (k & 1)  total += term;   /* odd k: + */
            else        total -= term;   /* even k: - */

            idx_t pent2 = k*(3*k + 1)/2;
            if (pent2 > m) continue;
            term = P[m - pent2];
            if (k & 1)  total += term;
            else        total -= term;
        }
        P[m] = total;
    }
    Psize = (size_t)(N+1);
}

seq_t a(idx_t n)
/* Return the number of partitions of n having rank ≥ 4. */
{
    if (n == 0) return 0;          /* offset starts at 1, safe guard */
    ensure_partitions(n);          /* need p[0..n] */

    seq_t result = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t shift = k*(3*k + 7)/2;
        if (shift > n) break;
        seq_t term = P[n - shift];
        if (k & 1)  result += term;   /* odd k: + */
        else        result -= term;   /* even k: - */
    }
    return result;
}
