/*
 * A290369  Number of partitions of n into parts that contain primes to odd
 *          powers only (A002035).
 * https://oeis.org/A290369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A290369: number of partitions of n into parts whose prime exponents are all odd. */
/* This implementation uses dynamic programming with a list of admissible part sizes. */

#include <stdint.h>
#include <stdlib.h>

/* external linkage for the sequence function */
seq_t a(idx_t n);

/* ----- internal static tables ----- */
static seq_t *dp      = NULL;  /* partition counts up to current max_n */
static size_t dp_cap  = 0;     /* allocated size of dp   */

static uint64_t *spf   = NULL; /* smallest prime factor table */
static size_t spf_cap  = 0;     /* allocated size of spf   */

static uint64_t *S     = NULL; /* admissible part sizes (odd‑exponent numbers) */
static size_t S_len    = 0;     /* number of admissible parts   */
static size_t S_cap    = 0;     /* allocated size of S          */

static idx_t max_n     = 0;     /* greatest n for which dp is up to date */

/* ----- helper: test if all prime exponents are odd ----- */
static int odd_exponents(uint64_t m)
{
    while (m > 1)
    {
        uint64_t p = spf[m];
        uint32_t c = 0;
        while (m % p == 0)
        {
            m /= p;
            ++c;
        }
        if ((c & 1U) == 0) return 0;          /* even exponent found */
    }
    return 1;
}

/* ----- rebuild all tables up to new_n (inclusive) ----- */
static void rebuild(idx_t new_n)
{
    size_t N = (size_t)new_n;                /* upper bound for int sizes */

    /* build smallest‑prime‑factor table up to N */
    if (spf_cap <= N)
    {
        free(spf);
        spf = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
        if (!spf) abort();
        spf_cap = N + 1;
        spf[1] = 1;
        for (size_t i = 2; i <= N; ++i)
        {
            if (spf[i] == 0)
            {
                spf[i] = (uint64_t)i;
                if (i * i <= N)
                {
                    for (size_t j = i * i; j <= N; j += i)
                        if (spf[j] == 0) spf[j] = (uint64_t)i;
                }
            }
        }
    }

    /* build list of admissible part sizes S (exclude 1) */
    if (S_cap <= N)
    {
        free(S);
        S = (uint64_t *)calloc(S_cap = N + 1, sizeof(uint64_t));
        if (!S) abort();
    }
    S_len = 0;
    for (size_t i = 2; i <= N; ++i)               /* parts start at 2 */
        if (odd_exponents((uint64_t)i))
            S[S_len++] = (uint64_t)i;

    /* allocate and compute DP table */
    free(dp);
    dp = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp) abort();
    dp[0] = (seq_t)1;      /* empty partition */

    for (size_t idx = 0; idx < S_len; ++idx)
    {
        size_t s = (size_t)S[idx];
        for (size_t k = s; k <= N; ++k)
            dp[k] += dp[k - s];
    }

    dp_cap = N + 1;
    max_n  = new_n;
}

/* ----- public sequence function ----- */
seq_t a(idx_t n)
{
    /* n is unsigned; no negative check required */
    if (!dp || (size_t)n >= dp_cap)
        rebuild(n);
    return dp[(size_t)n];
}
