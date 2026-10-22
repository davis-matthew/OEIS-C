/*
 * A332725  Heinz numbers of integer partitions whose negated first
 *          differences are not unimodal.
 * https://oeis.org/A332725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A332725 – Heinz numbers of integer partitions whose negated first
   differences are not unimodal.  We find the nth integer whose
   partition (obtained from the prime‑factorization of the integer)
   has negated first differences that are not unimodal.  This integer
   is itself the Heinz number of that partition. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/*  Global data – sieve, smallest prime factor and prime indices        */
/* --------------------------------------------------------------------- */
static const uint32_t SIEVE_LIMIT = 2000000;          /* adjust if more terms are needed */
static uint32_t *spf = NULL;                         /* smallest prime factor for each integer */
static uint32_t *prime_idx = NULL;                   /* 1‑based index of each prime */
static uint32_t *primes  = NULL;                     /* list of primes */
static size_t    prime_count = 0;
static bool      sieve_ready = false;

/* --------------------------------------------------------------------- */
/*  Build the sieve and auxiliary arrays (called once).                  */
/* --------------------------------------------------------------------- */
static void init_sieve(void)
{
    if (sieve_ready) return;

    spf        = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    prime_idx  = (uint32_t *)calloc(SIEVE_LIMIT + 1, sizeof(uint32_t));
    primes     = (uint32_t *)malloc((SIEVE_LIMIT / 10) * sizeof(uint32_t)); /* rough estimate */

    if (!spf || !prime_idx || !primes) return;  /* failure; leave data null */

    for (uint32_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (spf[i] == 0) {          /* i is prime */
            spf[i] = i;
            prime_idx[i] = (uint32_t)(++prime_count);
            primes[prime_count - 1] = i;

            if ((uint64_t)i * i <= SIEVE_LIMIT) {
                for (uint64_t j = (uint64_t)i * i; j <= SIEVE_LIMIT; j += i)
                    if (spf[(uint32_t)j] == 0) spf[(uint32_t)j] = i;
            }
        }
    }
    sieve_ready = true;
}

/* --------------------------------------------------------------------- */
/*  Test whether a non‑negative integer array is unimodal.               */
/* --------------------------------------------------------------------- */
static bool is_unimodal(const int *d, size_t sz)
{
    if (sz <= 1) return true;            /* empty or one element is trivially unimodal */
    size_t i = 0;
    while (i + 1 < sz && d[i] <= d[i + 1]) ++i;   /* increasing part */
    while (i + 1 < sz && d[i] >= d[i + 1]) ++i;   /* decreasing part */
    return i == sz - 1;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function.                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_sieve();                        /* sieve and tables ready */

    idx_t found = 0;                     /* how many suitable numbers found */
    uint64_t cand = 2;                    /* candidate integers start at 2 */

    for (;; ++cand) {
        /* factor cand and collect prime indices (partition parts) in descending order */
        uint64_t tmp = cand;
        int idxs[64];
        size_t idx_len = 0;

        if (tmp <= SIEVE_LIMIT) {
            while (tmp > 1) {
                uint32_t p = spf[(uint32_t)tmp];
                uint32_t pidx = prime_idx[p];
                while (tmp % p == 0) {            /* count exponent */
                    tmp /= p;
                    idxs[idx_len++] = (int)pidx;
                }
            }
        } else {   /* fallback trial division – unlikely for n ≤ 10 000 */
            for (size_t k = 0; k < prime_count && (uint64_t)primes[k] * primes[k] <= tmp; ++k) {
                uint32_t p = primes[k];
                if (tmp % p == 0) {
                    uint32_t pidx = prime_idx[p];
                    while (tmp % p == 0) {
                        tmp /= p;
                        idxs[idx_len++] = (int)pidx;
                    }
                }
            }
            if (tmp > 1) {                 /* remaining prime factor */
                uint32_t pidx = prime_idx[(uint32_t)tmp];
                idxs[idx_len++] = (int)pidx;
            }
        }

        /* insertion sort in descending order */
        for (size_t i = 1; i < idx_len; ++i) {
            int key = idxs[i];
            size_t j = i;
            while (j > 0 && idxs[j - 1] < key) {
                idxs[j] = idxs[j - 1];
                --j;
            }
            idxs[j] = key;
        }

        /* build negated first differences: p_i - p_{i+1} */
        int diffs[64];
        size_t diff_len = 0;
        if (idx_len >= 2) {
            for (size_t i = 0; i + 1 < idx_len; ++i)
                diffs[diff_len++] = idxs[i] - idxs[i + 1];
        }

        /* if the differences are NOT unimodal, accept this integer */
        if (!is_unimodal(diffs, diff_len)) {
            ++found;
            if (found == n) {
                return (seq_t)cand;        /* cand itself is the Heinz number */
            }
        }
    }
}
