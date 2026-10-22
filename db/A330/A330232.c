/*
 * A330232  MM-numbers of achiral multisets of multisets.
 * https://oeis.org/A330232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A330232 – MM‑numbers of achiral multisets of multisets.
   The n‑th term a(n) is the n‑th integer whose MM‑multiset of multisets
   is achiral.  A number k is MM‑achiral iff, after forming the multiset
   of prime indices of k and then for each such index taking its own
   prime‑index multiset, the resulting multiset of multisets is unchanged
   by any permutation of the indices.  This happens precisely when every
   index appearing in the inner multisets occurs the same total number
   of times.  The code implements this test and enumerates the sequence. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*---------------------------------------------------------------------*/
/*  Parameters for the sieve                                            */
/*---------------------------------------------------------------------*/
#define MAX_SIEVE 2000000   /* adequate for n up to ~10^5 or more */

/*---------------------------------------------------------------------*/
/*  Global sieve tables                                                */
/*---------------------------------------------------------------------*/
static bool sieve_initialized = false;
static int  *spf = NULL;                     /* smallest prime factor   */
static int  *prime_index_arr = NULL;          /* mapping prime -> index (1‑based) */
static int  *primes = NULL;                   /* list of primes          */
static int  prime_count = 0;                  /* number of primes found   */

/*---------------------------------------------------------------------*/
/*  Tables used by the achirality test                                 */
/*---------------------------------------------------------------------*/
static int *inner_counts = NULL;              /* counts of each index j */
static int *used_indices = NULL;              /* indices that got non‑zero counts  */
static int  used_capacity = 0;

/*---------------------------------------------------------------------*/
/*  Initialise the sieve and ancillary tables                          */
/*---------------------------------------------------------------------*/
static void init_sieve(void)
{
    if (sieve_initialized) return;
    sieve_initialized = true;

    /* allocate arrays */
    spf = (int*)calloc(MAX_SIEVE + 1, sizeof(int));
    prime_index_arr = (int*)calloc(MAX_SIEVE + 1, sizeof(int));
    primes = (int*)malloc((MAX_SIEVE + 1) * sizeof(int));  /* upper bound */

    /* linear sieve to generate primes and fill prime_index_arr   */
    int *linear = (int*)calloc(MAX_SIEVE + 1, sizeof(int));
    int pc = 0;
    for (int i = 2; i <= MAX_SIEVE; ++i) {
        if (linear[i] == 0) {
            linear[i] = i;
            primes[pc] = i;
            prime_count = ++pc;
            prime_index_arr[i] = prime_count;    /* 1‑based indices */
        }
        int pi = 0;
        int li = linear[i];
        while (pi < prime_count &&
               primes[pi] <= li &&
               (long long)i * primes[pi] <= MAX_SIEVE) {
            linear[i * primes[pi]] = primes[pi];
            ++pi;
        }
    }
    free(linear);

    /* initialise spf array */
    for (int i = 2; i <= MAX_SIEVE; ++i) spi_clear:
        if (spf[i] == 0) {
            for (long long j = i; j <= MAX_SIEVE; j += i)
                if (spf[(int)j] == 0) spf[(int)j] = i;
        }

    /* arrays for achirality test */
    inner_counts   = (int*)calloc(prime_count + 1, sizeof(int));
    used_capacity  = prime_count + 1;
    used_indices   = (int*)malloc(used_capacity * sizeof(int));
}

/*---------------------------------------------------------------------*/
/*  Factorisation helper: factor a number using spf (n <= MAX_SIEVE)  */
/*---------------------------------------------------------------------*/
static void factor_using_spf(idx_t n, int *cid, int *used, int *used_len)
{
    *used_len = 0;
    while (n > 1) {
        int p  = spf[(int)n];
        int exp = 0;
        while (n % p == 0) { ++exp; n /= p; }
        int idx = prime_index_arr[p];
        if (idx == 0) continue;                  /* should not happen */
        if (cid[idx] == 0) used[(*used_len)++] = idx;
        cid[idx] += exp;
    }
}

/*---------------------------------------------------------------------*/
/*  Test whether a given integer is MM‑achiral                        */
/*---------------------------------------------------------------------*/
static bool is_achiral(idx_t n)
{
    init_sieve();

    int *cnt    = inner_counts;
    int *us     = used_indices;
    int  used_sz = 0;

    /* factor the number n, each prime factor provides an outer index */
    idx_t temp = n;
    while (temp > 1) {
        int  p  = spf[(int)temp];
        int  exp_outer = 0;
        while (temp % p == 0) { ++exp_outer; temp /= p; }
        int idx_outer = prime_index_arr[p];
        if (idx_outer == 0) continue;           /* guard, should not happen */

        /* factor the outer index itself to get the inner multiset */
        idx_t ii = idx_outer;
        if (ii <= 1) continue;                  /* empty inner multiset */

        idx_t tmp = ii;
        while (tmp > 1) {
            int q  = spf[(int)tmp];
            int exp_inner = 0;
            while (tmp % q == 0) { ++exp_inner; tmp /= q; }
            int idx_q = prime_index_arr[q];
            if (idx_q == 0) continue;           /* safety */

            int add = (int)(exp_outer * exp_inner);
            if (cnt[idx_q] == 0) us[used_sz++] = idx_q;
            cnt[idx_q] += add;
        }
    }

    /* check that all non‑zero counts are equal */
    if (used_sz == 0) return true;              /* empty inner multisets */

    int base = cnt[us[0]];
    for (int i = 1; i < used_sz; ++i) {
        if (cnt[us[i]] != base) {
            for (int k = 0; k < used_sz; ++k) cnt[us[k]] = 0;
            return false;
        }
    }

    /* reset counts for next call */
    for (int k = 0; k < used_sz; ++k) cnt[us[k]] = 0;
    return true;
}

/*---------------------------------------------------------------------*/
/*  Sequence function a(n)                                            */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* static state for caching computed achiral numbers */
    static idx_t *achiral_seq = NULL;
    static size_t achiral_len = 0;
    static size_t achiral_cap = 0;
    static idx_t next_candidate = 1;

    init_sieve();

    while (achiral_len < n) {
        if (is_achiral(next_candidate)) {
            if (achiral_len == achiral_cap) {
                achiral_cap = achiral_cap ? achiral_cap * 2 : 16;
                achiral_seq = (idx_t*)realloc(achiral_seq,
                                              achiral_cap * sizeof(idx_t));
            }
            achiral_seq[achiral_len++] = next_candidate;
        }
        ++next_candidate;
    }

    return (seq_t)achiral_seq[n - 1];
}
