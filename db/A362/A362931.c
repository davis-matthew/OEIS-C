/*
 * A362931  a(n) = number of pairs (i,j) in [1..n] X [1..n] with integral
 *          harmonic mean 2*i*j/(i+j).
 * https://oeis.org/A362931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Sum_{1<=i<j<=n, (i+j)|(2*i*j)} 2
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes smallest prime factors up to 2*n
   and then uses that to efficiently count all pairs (i,j), i<j,
   for which the harmonic mean 2*i*j/(i+j) is an integer.  It grows
   the tables only when a larger n is requested. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Global tables for smallest prime factors (spf) and minimal
   L such that L*L is divisible by (m / gcd(m,2)).  They are
   built up to max_m_prepared and expanded if a larger 2*n is needed. */
static uint32_t *spf = NULL;
static uint32_t *Ls  = NULL;
static idx_t max_m_prepared = 0;

/* Build spurts up to M = 2*n.  The cost is O(M log log M). */
static void build_tables(idx_t M)
{
    /* Free any existing tables. */
    free(spf);
    free(Ls);

    spf = (uint32_t *)malloc((M + 1) * sizeof(uint32_t));
    Ls  = (uint32_t *)malloc((M + 1) * sizeof(uint32_t));
    if (!spf || !Ls) abort();

    /* Initialize spf array. */
    for (idx_t i = 0; i <= M; ++i) spf[i] = (uint32_t)i;

    /* Sieve for smallest prime factor. */
    for (idx_t i = 2; i * i <= M; ++i) {
        if (spf[i] == i) {
            for (idx_t j = i * i; j <= M; j += i)
                if (spf[j] == j) spf[j] = (uint32_t)i;
        }
    }

    /* Compute Ls table. */
    Ls[0] = 0;
    Ls[1] = 1;
    for (idx_t m = 2; m <= M; ++m) {
        uint32_t g = (m & 1) ? 1 : 2;          /* gcd(m,2) */
        uint32_t x = (uint32_t)(m / g);        /* m / gcd(m,2) */
        uint32_t res = 1;
        while (x > 1) {
            uint32_t p = spf[x];
            uint32_t e = 0;
            while (x % p == 0) {
                ++e;
                x /= p;
            }
            uint32_t exp = (e + 1) / 2;         /* ceil(e/2) */
            uint32_t powp = 1;
            for (uint32_t k = 0; k < exp; ++k) powp *= p;
            res *= powp;
        }
        Ls[m] = res;
    }

    max_m_prepared = M;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n<1, but harmless */

    /* Ensure tables are prepared up to 2*n. */
    idx_t needed = 2 * n;
    if (max_m_prepared < needed)
        build_tables(needed);

    seq_t total = (seq_t)n;          /* diagonal pairs (i,i) */

    /* Enumerate all sums m = i + j, 2 <= m <= 2*n. */
    for (idx_t m = 2; m <= 2 * n; ++m) {
        /* Range of i satisfying 1 <= i < j <= n and i + j = m. */
        idx_t low  = (m > n) ? (m - n) : 1;
        idx_t high = (m - 1) / 2;          /* automatically <= n-1 */

        if (low > high) continue;

        uint32_t L = Ls[(size_t)m];
        if (L == 0) continue;             /* should not happen */

        /* First multiple of L not less than low. */
        idx_t first = ((low + L - 1) / L) * (idx_t)L;
        if (first > high) continue;

        seq_t cnt = (seq_t)((high - first) / L + 1);
        total += cnt * 2;                  /* count both (i,j) and (j,i) */
    }

    return total;
}
