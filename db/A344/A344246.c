/*
 * A344246  Number of partitions of n into 6 semiprime parts.
 * https://oeis.org/A344246
 *
 * Offset:    24    (a(24) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered 6‑tuples of semiprimes that sum to n
 *            computed by dynamic programming over the list of semiprimes up
 *            to n
 * Range:     u8 n <= 90, u16 n <= 313, u32 n <= 3483, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static seq_t *dp = NULL;          /* DP table: dp[j*stride + t] = ways to use j parts for sum t */
static size_t dp_cap = 0;          /* allocated size of dp array (elements) */

static uint32_t *spf = NULL;      /* smallest prime factor for each number */
static size_t spf_cap = 0;        /* allocated size of spf array */

static uint64_t *semiprimes = NULL;   /* list of semiprimes up to current max_n */
static size_t semip_cap = 0;           /* allocated capacity for semiprimes */
static size_t semip_cnt = 0;           /* actual number of semiprimes */

static idx_t max_n = 0;            /* largest n for which we have computed dp */

/* Recompute all tables up to new_n (inclusive).  Assumes new_n > max_n. */
static void recompute(idx_t new_n)
{
    /* ---- allocate/resize dp ------------------------------------------------ */
    size_t dp_size = (size_t)(new_n + 1) * 7;
    if (dp_cap < dp_size) {
        free(dp);
        dp = (seq_t*)malloc(dp_size * sizeof(seq_t));
        dp_cap = dp_size;
    }

    /* ---- allocate/resize spf --------------------------------------------- */
    if (spf_cap < (size_t)new_n + 1) {
        free(spf);
        spf = (uint32_t*)malloc((new_n + 1) * sizeof(uint32_t));
        spf_cap = new_n + 1;
    }
    /* initialize spf */
    for (size_t i = 0; i < spf_cap; ++i) spf[i] = 0;

    /* Sieve to fill smallest prime factor */
    for (idx_t i = 2; i <= new_n; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= new_n) {
                for (idx_t j = i * i; j <= new_n; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }
    }

    /* ---- build semiprime list --------------------------------------------- */
    if (semip_cap < (size_t)new_n + 1) {
        free(semiprimes);
        semiprimes = (uint64_t*)malloc((new_n + 1) * sizeof(uint64_t));
        semip_cap = new_n + 1;
    }
    semip_cnt = 0;
    for (idx_t x = 2; x <= new_n; ++x) {
        uint8_t omega = 0;
        idx_t tmp = x;
        while (tmp > 1) {
            uint32_t p = spf[tmp];
            tmp /= p;
            ++omega;
        }
        if (omega == 2) {
            semiprimes[semip_cnt++] = (uint64_t)x;
        }
    }

    /* ---- DP initialization -------------------------------------------------- */
    for (size_t i = 0; i < dp_size; ++i) dp[i] = 0;
    size_t stride = (size_t)new_n + 1;
    dp[0 * stride + 0] = (seq_t)1;   /* zero parts sum to zero */

    /* ---- DP over semiprimes ---------------------------------------------- */
    for (size_t s_idx = 0; s_idx < semip_cnt; ++s_idx) {
        uint32_t s = (uint32_t)semiprimes[s_idx];
        for (unsigned int j = 1; j <= 6; ++j) {
            seq_t* dp_j   = dp + j * stride;
            seq_t* dp_jm1 = dp + (j - 1) * stride;
            for (idx_t t = s; t <= new_n; ++t) {
                dp_j[t] += dp_jm1[t - s];
            }
        }
    }

    max_n = new_n;
}

/* Public function implementing the OEIS sequence */
seq_t a(idx_t n)
{
    if (n < 24) return (seq_t)0;   /* values below the offset are unspecified */
    if (n > max_n) recompute(n);
    size_t stride = (size_t)max_n + 1;
    return dp[6 * stride + n];
}
