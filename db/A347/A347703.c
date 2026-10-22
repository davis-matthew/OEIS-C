/*
 * A347703  Number of compositions (ordered partitions) of n into at most 4
 *          prime parts (counting 1 as a prime).
 * https://oeis.org/A347703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into at most 4 prime parts
 *            (counting 1 as a prime)
 * Range:     u8 n <= 27, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a simple dynamic‑programming recurrence.
   For every k = 1..4 (number of parts) we compute
      dp_k[i] = sum_{p prime, p≤i} dp_{k-1}[i‑p]   with dp_0[0] = 1.
   The answer a(n) is the sum over k (including k = 0 for n = 0). */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Prime sieve (cached) ---------- */
static uint64_t *prime_list  = NULL; /* primes > 1, sorted */
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0;      /* maximum value for which primes are generated */

/* Allocate & fill prime_list with all primes ≤ limit (1 is *not* included). */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* free previous list */
    free(prime_list);
    prime_list = NULL;
    prime_cnt  = 0;
    prime_limit = limit;

    size_t sz = (size_t)limit + 1;
    uint8_t *is_comp = (uint8_t *)calloc(sz, sizeof(uint8_t));
    if (!is_comp)
        return; /* allocation failure – will not happen in tests */

    /* temporary buffer for primes */
    size_t tmp_cap = 64;
    uint64_t *tmp = (uint64_t *)malloc(tmp_cap * sizeof(uint64_t));
    if (!tmp) {
        free(is_comp);
        return;
    }

    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_comp[(size_t)i]) {
            /* i is prime */
            if (prime_cnt >= tmp_cap) {
                tmp_cap <<= 1;
                uint64_t *newtmp = (uint64_t *)realloc(tmp, tmp_cap * sizeof(uint64_t));
                if (!newtmp) {
                    free(tmp);
                    free(is_comp);
                    return;
                }
                tmp = newtmp;
            }
            tmp[prime_cnt++] = (uint64_t)i;

            if ((uint64_t)i * (uint64_t)i <= (uint64_t)limit) {
                for (uint64_t j = (uint64_t)i * (uint64_t)i;
                     j <= (uint64_t)limit; j += (uint64_t)i)
                    is_comp[(size_t)j] = 1;
            }
        }
    }

    free(is_comp);
    prime_list = tmp; /* ownership */
}

/* ---------- Cache for the sequence ---------- */
static seq_t *a_cache = NULL; /* a_cache[n] = a(n) for 0..a_size-1 */
static size_t a_size = 0;     /* number of computed indices */

/* Recompute the sequence up to 'n' (inclusive) and store it in a_cache. */
static void
recompute_cache(idx_t n)
{
    if (n < a_size)
        return; /* already have values up to n */

    /* get primes up to n (excluding 1) */
    ensure_primes(n);

    size_t np = (size_t)(n + 1);

    /* arrays for dynamic programming */
    seq_t *dp   = (seq_t *)calloc(np, sizeof(seq_t));
    seq_t *ans  = (seq_t *)calloc(np, sizeof(seq_t));
    if (!dp || !ans) {
        free(dp); free(ans);
        return;
    }

    dp[0] = 1;          /* dp$_0$ for k=0 parts */
    ans[0] = 1;         /* a(0) = 1 */

    /* iterations for k = 1..4 parts */
    for (size_t k = 1; k <= 4; ++k) {
        seq_t *newdp = (seq_t *)calloc(np, sizeof(seq_t));
        if (!newdp) {
            free(dp);
            free(ans);
            return;
        }

        for (idx_t i = 1; i <= n; ++i) {
            seq_t sum = dp[(size_t)(i - 1)];      /* part 1 (counting 1 as prime) */

            for (size_t pi = 0; pi < prime_cnt && prime_list[pi] <= (uint64_t)i; ++pi)
                sum += dp[(size_t)(i - (size_t)prime_list[pi])];

            newdp[(size_t)i] = sum;
            ans[(size_t)i] += sum;                 /* accumulate contributions from all k */
        }

        free(dp);
        dp = newdp;
    }

    /* replace old cache */
    free(a_cache);
    a_cache = ans;
    a_size  = np;

    free(dp); /* free the last dp array (k=4 results are not needed) */
}

/* ---------- Public API ---------- */
seq_t
a(idx_t n)
{
    if ((idx_t)(a_size) <= n) /* need more values */
        recompute_cache(n);

    return a_cache[n];
}
