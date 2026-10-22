/*
 * A331781  Triangle read by rows: T(m,n) = Sum_{0<i<m, 0<j<n, gcd{i,j}=1} 1,
 *          where m >= n >= 1.
 * https://oeis.org/A331781
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------
   OEIS A331781 – Triangle read by rows
   ------------------------------------------------------------
   T(m,n) = Σ_{1≤i<m, 1≤j<n, gcd(i,j)=1} 1 .
   The sequence a(n) enumerates these triangle entries row by row,
   starting with n = 1.
   ------------------------------------------------------------
   Formula used:
       a(n) = Σ_{d=1}^{min(M,N)} μ(d) * ⌊M/d⌋ * ⌊N/d⌋
   where M = m-1, N = n-1, and μ is the Möbius function.
   ------------------------------------------------------------
   The code below computes μ up to the required maximum
   on demand with a simple linear sieve and then evaluates
   the above sum.  All arithmetic is performed with
   signed 64‑bit integers for the intermediate sum; the
   final result is cast to seq_t.
   ------------------------------------------------------------ */

#include <stdlib.h>
#include <stdint.h>

static int  *mu     = NULL;   /* Möbius values */
static size_t mu_cap = 0;     /* current capacity of mu */

/* ------------------------------------------------------------
   Ensure that μ(1..limit) are available.  The function
   recomputes all values up to limit each time it is called
   with a larger limit.  Since limit is at most ~10 000,
   this is easily fast enough.
   ------------------------------------------------------------ */
static void
ensure_mu(size_t limit)
{
    if (limit + 1 <= mu_cap)
        return;                     /* already available */

    /* resize the μ array */
    size_t new_cap = limit + 1;
    int *new_mu = (int*)realloc(mu, new_cap * sizeof(int));
    if (!new_mu)
        abort();
    mu = new_mu;
    for (size_t i = mu_cap; i < new_cap; ++i)
        mu[i] = 0;
    mu_cap = new_cap;

    /* auxiliary arrays for the linear sieve */
    static uint32_t *primes = NULL;
    static size_t    primes_cap = 0;
    static size_t    primes_cnt = 0;
    static uint8_t  *is_comp = NULL;
    static size_t    comp_cap = 0;

    if (primes_cap < new_cap) {
        uint32_t *new_pr = (uint32_t*)realloc(primes, new_cap * sizeof(uint32_t));
        if (!new_pr)
            abort();
        primes = new_pr;
        primes_cap = new_cap;
    }

    if (comp_cap < new_cap) {
        uint8_t *new_com = (uint8_t*)realloc(is_comp, new_cap * sizeof(uint8_t));
        if (!new_com)
            abort();
        is_comp = new_com;
        comp_cap = new_cap;
    }

    /* initialise */
    for (size_t i = 0; i < new_cap; ++i)
        is_comp[i] = 0;

    mu[0] = 0;
    if (new_cap > 1)
        mu[1] = 1;
    primes_cnt = 0;

    for (size_t i = 2; i < new_cap; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = (uint32_t)i;
            mu[i] = -1;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = primes[j];
            size_t ip = i * p;
            if (ip >= new_cap)
                break;
            is_comp[ip] = 1;
            if (i % p == 0) {
                mu[ip] = 0;
                break;
            } else {
                mu[ip] = -mu[i];
            }
        }
    }
}

/* ------------------------------------------------------------
   Main sequence function requested by the build system.
   ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)               /* protective; OEIS offset is 1 */
        return (seq_t)0;

    /* --- decode n into triangle coordinates (m,k) --- */
    idx_t m = 1;
    while (m * (m + 1) / 2 < n)
        ++m;                   /* m satisfies m*(m+1)/2 ≥ n */
    idx_t k = n - (m * (m - 1) / 2);   /* 1 ≤ k ≤ m */

    /* parameters for counting coprime pairs */
    idx_t M = m - 1;
    idx_t N = k - 1;
    if (M == 0 || N == 0)
        return (seq_t)0;       /* empty sum */

    /* build μ up to N if needed */
    ensure_mu((size_t)N);

    /* compute the Möbius sum */
    long long sum = 0;
    for (size_t d = 1; d <= (size_t)N; ++d) {
        int mu_d = mu[d];
        if (mu_d == 0)
            continue;
        sum += mu_d * (long long)(M / d) * (long long)(N / d);
    }

    return (seq_t)sum;
}
