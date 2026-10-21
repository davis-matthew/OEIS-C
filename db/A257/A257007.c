/*
 * A257007  Number of Zagier-reduced binary quadratic forms of discriminant
 *          n^2-4.
 * https://oeis.org/A257007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{b=1}^{n-1} dbm(b, n*b - b^2 - 1) * where dbm(b, m)
 *            is the number of positive divisors of m that are < b. * * This
 *            implementation precomputes smallest prime factors (SPF) up to
 *            the * maximum value needed for the given n. For each b it
 *            factors * m = n*b - b^2 - 1 using SPF, enumerates a...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Shared global state for smallest prime factors */
static uint32_t *spf = NULL;       /* array of smallest prime factors */
static idx_t    spf_max = 0;       /* max index currently precomputed */

/* Ensure that spf[0..max_val] is computed.  Rebuilds the table
 * from scratch whenever a larger limit is requested. */
static void
ensure_spf_up_to(idx_t max_val)
{
    if (max_val <= spf_max)
        return;

    free(spf);
    spf = (uint32_t *)malloc((max_val + 1) * sizeof(uint32_t));
    for (idx_t i = 0; i <= max_val; ++i)
        spf[i] = 0;

    for (idx_t i = 2; i <= max_val; ++i) {
        if (spf[i] == 0) {              /* i is prime */
            spf[i] = (uint32_t)i;
            if (i > max_val / i)
                continue;
            for (idx_t j = i * i; j <= max_val; j += i)
                if (spf[j] == 0)
                    spf[j] = (uint32_t)i;
        }
    }
    spf_max = max_val;
}

/* Recursive helper that enumerates all divisors of a number
 * from its prime factorisation (prime[i]^exp[i]) and counts
 * those strictly less than 'b'. */
static void
dfs_count(int idx, uint64_t curr,
          const idx_t primes[], const int exps[],
          int num, idx_t b, uint32_t *cnt)
{
    if (idx == num) {
        if (curr < b)
            (*cnt)++;
        return;
    }
    idx_t p = primes[idx];
    int e  = exps[idx];
    uint64_t v = curr;
    for (int i = 0; i <= e; ++i) {
        dfs_count(idx + 1, v, primes, exps, num, b, cnt);
        v *= p;
    }
}

/* Count divisors of m that are < b using the SPF table. */
static uint32_t
count_div_smaller(idx_t b, idx_t m)
{
    if (m == 0)
        return 0;

    /* factorisation */
    idx_t primes[16];
    int  exps[16];
    int  num = 0;
    idx_t mm = m;
    while (mm > 1 && num < 16) {
        idx_t p = spf[mm];
        int   e = 0;
        do { mm /= p; ++e; } while (mm > 1 && spf[mm] == p);
        primes[num] = p;
        exps[num]   = e;
        ++num;
    }

    uint32_t cnt = 0;
    dfs_count(0, 1ULL, primes, exps, num, b, &cnt);
    return cnt;
}

/*--------------------------------------------------------------------
 * Public sequence function
 *--------------------------------------------------------------------*/
seq_t
a(idx_t n)
{
    /* Values for n <= 2 are zero */
    if (n <= 2)
        return 0;

    /* Compute the largest m needed for this n:
     *   m = b*(n-b) - 1  (max at b = floor(n/2))
     */
    idx_t max_b = n / 2;
    idx_t max_m = max_b * ((n - max_b)) - 1;
    if (max_m < 1)
        max_m = 1;            /* ensure at least one element */

    ensure_spf_up_to(max_m);

    seq_t sum = 0;
    for (idx_t b = 1; b < n; ++b) {
        /* m = n*b - b*b - 1 is always non‑negative for 1 <= b < n */
        idx_t m = n * b - b * b - 1u;
        if (m == 0)
            continue;
        seq_t cnt = count_div_smaller(b, m);
        sum += cnt;
    }
    return sum;
}
