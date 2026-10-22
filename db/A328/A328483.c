/*
 * A328483  Maximum number of times any term appears when n is expressed as a
 *          greedy sum of terms of A129912 (maximal digit when n is expressed
 *          in greedy A129912-base).
 * https://oeis.org/A328483
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1061, u16 n <= 9931, u32 n <= 9931, u64 n <= 9931,
 *            u128 n <= 9931, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the maximum multiplicity of any term when n is represented
   greedily as a sum of terms from A129912 (products of distinct
   primorials).  The greedy algorithm chooses the largest admissible
   term repeatedly.  The function works for any idx_t and seq_t (including
   arbitrary‑precision integer classes) and is valid in C11 and C++17. */
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* Forward declaration of a helper */
static int cmp_seq(const void *a, const void *b);

/* Global cache for the A129912 terms */
static seq_t *S = NULL;          /* sorted array of terms */
static size_t Slen = 0;          /* number of terms stored */
static idx_t cache_max = 0;      /* largest n for which S is valid */

/* Comparator for qsort on seq_t */
static int cmp_seq(const void *a, const void *b)
{
    const seq_t v1 = *(const seq_t *)a;
    const seq_t v2 = *(const seq_t *)b;
    if (v1 < v2) return -1;
    if (v1 > v2) return 1;
    return 0;
}

/* Generate all A129912 terms up to 'max_n' */
static void generate_terms(idx_t max_n)
{
    seq_t limit = (seq_t)max_n;
    if (limit < 1) {
        /* nothing to do */
        return;
    }

    /* Small list of primes – 50 primes are more than enough for n <= 10000 */
    static const uint64_t primes_list[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL, 71ULL,
        73ULL, 79ULL, 83ULL, 89ULL, 97ULL, 101ULL, 103ULL, 107ULL, 109ULL, 113ULL,
        127ULL, 131ULL, 137ULL, 139ULL, 149ULL, 151ULL, 157ULL, 163ULL, 167ULL, 173ULL,
        179ULL, 181ULL, 191ULL, 193ULL, 197ULL, 199ULL, 211ULL, 223ULL, 227ULL, 229ULL,
        233ULL, 239ULL, 241ULL, 251ULL, 257ULL, 263ULL, 269ULL, 271ULL, 277ULL, 281ULL
    };
    const size_t primes_cnt = sizeof(primes_list) / sizeof(primes_list[0]);

    /* Generate primorials up to the limit */
    seq_t primorials[primes_cnt];
    size_t prim_cnt = 0;
    seq_t prod = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        prod *= (seq_t)primes_list[i];
        if (prod > limit) break;
        primorials[prim_cnt++] = prod;
    }

    /* Generate all subset products of the primorials (including 1). */
    size_t cap = 64;
    seq_t *subset = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!subset) abort();
    size_t subset_cnt = 0;
    subset[subset_cnt++] = (seq_t)1;   /* empty product */

    for (size_t pi = 0; pi < prim_cnt; ++pi) {
        seq_t p = primorials[pi];
        size_t cur = subset_cnt;
        for (size_t j = 0; j < cur; ++j) {
            /* avoid overflow: check before multiplication */
            if (subset[j] <= limit / p) {
                seq_t val = subset[j] * p;
                if (subset_cnt == cap) {
                    cap *= 2;
                    subset = (seq_t *)realloc(subset, cap * sizeof(seq_t));
                    if (!subset) abort();
                }
                subset[subset_cnt++] = val;
            }
        }
    }

    /* Sort and deduplicate */
    qsort(subset, subset_cnt, sizeof(seq_t), cmp_seq);
    size_t uniq = 0;
    for (size_t i = 0; i < subset_cnt; ++i) {
        if (uniq == 0 || subset[i] != subset[uniq - 1]) {
            subset[uniq++] = subset[i];
        }
    }

    /* Allocate the new cache and replace the old one */
    seq_t *newS = (seq_t *)malloc(uniq * sizeof(seq_t));
    if (!newS) abort();
    memcpy(newS, subset, uniq * sizeof(seq_t));
    free(subset);

    free(S);
    S = newS;
    Slen = uniq;
    cache_max = max_n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Offset 0 */
    if (n == 0) return (seq_t)0;

    /* Ensure we have precomputed terms up to n */
    if (n > cache_max) {
        generate_terms(n);
    }

    seq_t rem = (seq_t)n;
    size_t pos = Slen - 1;
    seq_t maxcnt = 0;

    while (rem > 0) {
        /* Find the largest term <= rem */
        while (pos > 0 && S[pos] > rem) pos--;
        seq_t t = S[pos];
        seq_t times = rem / t;
        if (times > maxcnt) maxcnt = times;
        rem -= times * t;
    }

    return maxcnt;
}
