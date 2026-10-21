/*
 * A295782  Square array A(n,k), n >= 1, k >= 1, read by antidiagonals, where
 *          A(n,k) is the number of coprime pairs (a,b) with -n <= a <= n, -k
 *          <= b <= k.
 * https://oeis.org/A295782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{d=1}^{max(i,j)} μ(d)·[(2·(i/d)+1)(2·(j/d)+1)−1] where
 *            (i,j) are the 1‑based antidiagonal coordinates of n. The array
 *            is read by antidiagonals: (1,1),(1,2),(2,1),(1,3),(2,2),(3,1),…
 * Range:     u8 n <= 177, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Möbius function table (mu[d] for 1 ≤ d ≤ mu_limit)                   */
/* --------------------------------------------------------------------- */
static int   *mu_table   = NULL;   /* mu[d]  (offset 1) */
static idx_t mu_limit   = 0;       /* current allocated size */

/* ensure that mu_table contains μ(d) for 1 ≤ d ≤ limit */
static void ensure_mu(idx_t limit)
{
    if (limit <= mu_limit)
        return;

    int *new_table = (int *)realloc(mu_table, (size_t)(limit + 1) * sizeof(int));
    if (!new_table)
        abort();                       /* allocation failure */

    /* compute μ(d) for newly allocated entries */
    for (idx_t d = mu_limit + 1; d <= limit; ++d) {
        if (d == 1) {
            new_table[d] = 1;
            continue;
        }
        uint64_t n = d;
        int cnt = 0;   /* number of distinct prime factors so far */
        int mu_val = 1;
        for (uint64_t p = 2; p * p <= n; ++p) {
            if (n % p == 0) {
                int exp = 0;
                while (n % p == 0) {
                    n /= p;
                    ++exp;
                }
                if (exp > 1) {
                    mu_val = 0;   /* square factor → μ=0 */
                    break;
                }
                ++cnt;
            }
        }
        if (mu_val != 0) {
            if (n > 1)      /* remaining prime factor */
                ++cnt;
            mu_val = (cnt & 1) ? -1 : 1;  /* parity of cnt */
        }
        new_table[d] = mu_val;
    }

    mu_table = new_table;
    mu_limit = limit;
}

/* --------------------------------------------------------------------- */
/*  Compute a(n) for n ≥ 1                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;     /* undefined for n=0, but return 0 safely */

    /* 1‑based antidiagonal coordinates (i,j) for index n              */
    idx_t i, j;
    /* find smallest s with s*(s+1)/2 >= n                               */
    idx_t s = 0;
    idx_t t = 0;   /* triangular number t = s*(s+1)/2 */
    while (t < n) {
        ++s;
        t += s;           /* t becomes s*(s+1)/2 after increment */
    }
    /* offset of n within this antidiagonal: 1 … s                         */
    idx_t offset = n - (t - s);          /* t-s = (s-1)*s/2                */
    i = offset;                          /* i ranges 1 … s                 */
    j = s - offset + 1;                  /* j ranges 1 … s                 */

    /* compute maximum of i and j (needed for Möbius sum)                  */
    idx_t maxi = (i > j) ? i : j;
    ensure_mu(maxi);                     /* make sure μ values are ready    */

    int64_t sum = 0;
    for (idx_t d = 1; d <= maxi; ++d) {
        int mu_d = mu_table[d];
        if (mu_d == 0)
            continue;
        idx_t cnt_i = 2 * (i / d) + 1;   /* number of a divisible by d  */
        idx_t cnt_j = 2 * (j / d) + 1;   /* number of b divisible by d  */
        int64_t term = (int64_t)cnt_i * (int64_t)cnt_j - 1;   /* subtract (0,0) */
        sum += (int64_t)mu_d * term;
    }

    return (seq_t)sum;
}
