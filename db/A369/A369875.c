/*
 * A369875  a(n) = [x^n] Product_{d|n} (x^d + 1 + 1/x^d).
 * https://oeis.org/A369875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to n = Σ_{d|n} c_i d, c_i∈{-1,0,1}
 * Range:     u8 n <= 47, u16 n <= 119, u32 n <= 719, u64 n <= 5039,
 *            u128 n <= 8724, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the count by dynamic programming over the divisors of n. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;          /* DP array for current sums */
static seq_t *newdp = NULL;       /* Temporary DP array */
static idx_t dp_cap = 0;          /* Capacity of the DP arrays (size = 2*n + 1) */

static void ensure_dp_cap(idx_t len)
{
    if (len <= dp_cap) return;
    if (dp)  free(dp);
    if (newdp) free(newdp);
    dp_cap = len;
    dp = (seq_t*)calloc(len, sizeof(seq_t));
    newdp = (seq_t*)calloc(len, sizeof(seq_t));
}

static void factorize(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    *cnt = 0;
    uint64_t m = n;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int e = 0;
            while (m % p == 0) { m /= p; ++e; }
            pr[(*cnt)] = p;
            ex[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (m > 1) {
        pr[(*cnt)] = m;
        ex[(*cnt)] = 1;
        ++(*cnt);
    }
}

static void generate_divisors(uint64_t *pr, int *ex, size_t cnt,
                              uint64_t *divs, size_t *div_len)
{
    divs[0] = 1;
    *div_len = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        int e = ex[i];
        size_t old_len = *div_len;
        uint64_t mult = 1;
        for (int k = 1; k <= e; ++k) {
            mult *= p;
            for (size_t j = 0; j < old_len; ++j) {
                divs[(*div_len)] = divs[j] * mult;
                ++(*div_len);
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* Off‑by‑one guard; not used at the offset 1 */

    /* Ensure DP arrays are large enough: size = 2*n + 1 */
    idx_t len = n * 2 + 1;
    ensure_dp_cap(len);

    /* Clear DP arrays */
    memset(dp, 0, len * sizeof(seq_t));
    memset(newdp, 0, len * sizeof(seq_t));

    idx_t offset = n;          /* index of sum = 0 */
    dp[offset] = 1;            /* one way to achieve sum 0 */

    /* Factor n to get its divisors */
    uint64_t primes[64];
    int exps[64];
    size_t factor_cnt;
    factorize((uint64_t)n, primes, exps, &factor_cnt);

    uint64_t divs[512];
    size_t div_len;
    generate_divisors(primes, exps, factor_cnt, divs, &div_len);

    /* Dynamic programming over the divisors */
    for (size_t di = 0; di < div_len; ++di) {
        uint64_t d = divs[di];
        idx_t id = (idx_t)d;          /* safe cast: d <= n <= idx_t */
        /* reset newdp */
        memset(newdp, 0, len * sizeof(seq_t));

        /* Update possibilities for this divisor */
        for (idx_t s = 0; s < len; ++s) {
            seq_t val = dp[s];
            if (val == 0) continue;

            /* coefficient 0 */
            newdp[s] += val;
            /* coefficient +d */
            if (s + id < len) newdp[s + id] += val;
            /* coefficient -d */
            if (s >= id)      newdp[s - id] += val;
        }

        /* swap dp and newdp for next iteration */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    /* Answer corresponds to sum = n → index offset + n = 2*n */
    return dp[2 * n];
}
