/*
 * A360681  Numbers for which the prime signature has the same median as the
 *          first differences of 0-prepended prime indices.
 * https://oeis.org/A360681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the nth integer whose median of prime‑exponents
 *            equals the median of the differences of the 0‑prepended prime
 *            indices (indices repeated by exponent)
 * Range:     u8 n <= 33, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int cmp_int(const void *a, const void *b)
{ return (*(int*)a - *(int*)b); }

/* Global data: sieve, prime list and sequence storage */
static uint32_t *spf = NULL;          /* smallest prime factor */
static uint32_t *prime_idx = NULL;    /* prime → 1‑based index in primes list */
static uint32_t primes[200000];       /* list of primes up to LIMIT */
static int prime_count = 0;

/* Sequence storage: seq[1] = 1, seq[2] = second term, etc. */
#define SEQ_CAP 200000
static seq_t seq[SEQ_CAP];
static idx_t seq_len = 1;   /* number of terms stored */

/* Sieve of Eratosthenes with linear factorisation support, up to limit */
static void build_sieve(int limit)
{
    spf = (uint32_t*)malloc((limit+1) * sizeof(uint32_t));
    prime_idx = (uint32_t*)malloc((limit+1) * sizeof(uint32_t));
    memset(spf, 0, (limit+1)*sizeof(uint32_t));
    memset(prime_idx, 0, (limit+1)*sizeof(uint32_t));

    prime_count = 0;
    for (int i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            prime_idx[i] = prime_count + 1;  /* 1‑based index */
            primes[prime_count++] = i;
        }
        for (int j = 0; j < prime_count; ++j) {
            uint32_t p = primes[j];
            uint64_t mul = (uint64_t)i * p;
            if (mul > (uint64_t)limit) break;
            spf[mul] = p;
            if (p == spf[i]) break;
        }
    }
}

/* Checks whether integer m satisfies the median condition. */
static int qualifies(uint32_t m)
{
    /* factorization */
    uint32_t x = m;
    int fac_cnt = 0;
    int exps[32];
    int idx_rep[64];
    int rep_cnt = 0;

    while (x > 1) {
        uint32_t p = spf[x];
        int e = 0;
        while (x % p == 0) {
            x /= p;
            e++;
        }
        exps[fac_cnt++] = e;
        for (int k = 0; k < e; ++k) {
            idx_rep[rep_cnt + k] = prime_idx[p];
        }
        rep_cnt += e;
    }

    /* median2 of exponents (numerator*2) */
    uint64_t median2_exps = 0;
    if (fac_cnt > 0) {
        qsort(exps, fac_cnt, sizeof(int), cmp_int);
        if (fac_cnt & 1)
            median2_exps = (uint64_t)exps[fac_cnt / 2] * 2;
        else
            median2_exps = (uint64_t)exps[fac_cnt / 2 - 1] + exps[fac_cnt / 2];
    }

    /* median2 of differences */
    uint64_t median2_diffs = 0;
    if (rep_cnt > 0) {
        qsort(idx_rep, rep_cnt, sizeof(int), cmp_int);
        int prev = 0;
        int diffs[64];
        int diff_cnt = 0;
        for (int i = 0; i < rep_cnt; ++i) {
            diffs[diff_cnt++] = idx_rep[i] - prev;
            prev = idx_rep[i];
        }
        qsort(diffs, diff_cnt, sizeof(int), cmp_int);
        if (diff_cnt & 1)
            median2_diffs = (uint64_t)diffs[diff_cnt / 2] * 2;
        else
            median2_diffs = (uint64_t)diffs[diff_cnt / 2 - 1] + diffs[diff_cnt / 2];
    }

    return median2_exps == median2_diffs;
}

seq_t a(idx_t n)
{
    /* one‑time initialisation */
    static int initialized = 0;
    if (!initialized) {
        /* build sieve up to a generous limit (2 000 000) */
        build_sieve(2000000);
        seq[1] = (seq_t)1;   /* a(1) = 1 */
        seq_len = 1;
        initialized = 1;
    }

    static idx_t checked = 1;   /* last number examined (1 already handled) */

    while (seq_len < n) {
        ++checked;
        if (qualifies((uint32_t)checked))
            seq[++seq_len] = (seq_t)checked;
    }
    return seq[n];
}
