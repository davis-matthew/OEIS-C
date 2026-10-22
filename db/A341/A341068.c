/*
 * A341068  Number of ways to write n as an ordered sum of 8 squarefree
 *          numbers.
 * https://oeis.org/A341068
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in (Sum_{k squarefree} x^k)^8
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* --------------  helper: integer square root -------------- */
static unsigned _isqrt_u64(idx_t n) {
    unsigned r = 0;
    while ((uint64_t)(r + 1) * (uint64_t)(r + 1) <= n)
        ++r;
    return r;
}

/* --------------  table for dynamic programming -------------- */
static seq_t *dp[9] = { NULL };        /* dp[m][i] – ways with m parts, sum i */
static idx_t dp_max = 0;                /* highest n currently computed */

/* --------------  rebuild tables up to new_n -------------- */
static void recompute(idx_t new_n) {
    idx_t i, k, m;
    unsigned sqrtN = _isqrt_u64(new_n);
    unsigned *primes = NULL;
    size_t pcnt = 0;

    /* 1. free old tables */
    for (m = 0; m <= 8; ++m) {
        if (dp[m]) {
            free(dp[m]);
            dp[m] = NULL;
        }
    }

    /* 2. allocate new tables */
    for (m = 0; m <= 8; ++m) {
        dp[m] = (seq_t *)calloc((size_t)new_n + 1, sizeof(seq_t));
        if (!dp[m]) abort();
    }
    dp[0][0] = (seq_t)1;          /* base case */

    /* 3. generate list of prime numbers up to sqrtN */
    if (sqrtN >= 2) {
        primes = (unsigned *)malloc((size_t)sqrtN * sizeof(unsigned));
        if (!primes) abort();
        for (unsigned p = 2; p <= sqrtN; ++p) {
            bool isprime = true;
            for (size_t j = 0; j < pcnt; ++j) {
                unsigned q = primes[j];
                if (q * q > p) break;
                if (p % q == 0) { isprime = false; break; }
            }
            if (isprime) primes[pcnt++] = p;
        }
    }

    /* 4. build square‑free indicator array */
    bool *sqf = (bool *)calloc((size_t)new_n + 1, sizeof(bool));
    if (!sqf) abort();
    for (k = 0; k <= new_n; ++k) sqf[k] = true;          /* 1 is square‑free */

    for (size_t pi = 0; pi < pcnt; ++pi) {
        unsigned p = primes[pi];
        unsigned sq = p * p;
        for (k = sq; k <= new_n; k += sq)
            sqf[k] = false;
    }

    /* 5. collect all square‑free numbers into a vector */
    unsigned *sqf_nums = (unsigned *)malloc((size_t)new_n * sizeof(unsigned));
    if (!sqf_nums) abort();
    size_t sqf_cnt = 0;
    for (k = 1; k <= new_n; ++k)
        if (sqf[k]) sqf_nums[sqf_cnt++] = (unsigned)k;

    /* 6. dynamic programming: iterate parts */
    for (m = 1; m <= 8; ++m) {
        seq_t *prev = dp[m - 1];
        seq_t *cur  = dp[m];
        for (size_t si = 0; si < sqf_cnt; ++si) {
            unsigned sqk = sqf_nums[si];
            for (i = sqk; i <= new_n; ++i) {
                cur[i] += prev[i - sqk];
            }
        }
    }

    /* 7. clean up */
    free(sqf);
    free(sqf_nums);
    if (primes) free(primes);

    dp_max = new_n;
}

/* --------------  public interface -------------- */
seq_t a(idx_t n) {
    if (n < 8) return (seq_t)0;          /* defined only for n ≥ 8 */

    if (n > dp_max || dp[8] == NULL)
        recompute(n);

    return dp[8][n];
}
