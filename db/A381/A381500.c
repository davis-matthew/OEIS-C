/*
 * A381500  a(n) = A019565(A187769(n)).
 * https://oeis.org/A381500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A019565(A187769(n)) * Sequence A381500: squarefree
 *            numbers sorted first by largest prime factor, * then by number
 *            of prime factors, then lexicographically (ascending) on the *
 *            already-descending list of prime factors. * * The implementation
 *            builds the required row using combinatorial * unr...
 * Range:     u8 n <= 25, u16 n <= 119, u32 n <= 1022, u64 n <= 9687,
 *            u128 n <= 9687, big any n
 * Helpers:   build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/build_primes_upto.h"

#define MAX_BINOM 64          /* supports N up to 63, sufficient for all n <= 2^63 */

static uint64_t C_binom[MAX_BINOM][MAX_BINOM];
static bool binom_inited = false;

static void init_binom(void)
{
    if (binom_inited) return;
    for (int n = 0; n < MAX_BINOM; ++n) {
        C_binom[n][0] = C_binom[n][n] = 1;
        for (int k = 1; k < n; ++k) {
            C_binom[n][k] = C_binom[n - 1][k - 1] + C_binom[n - 1][k];
        }
    }
    binom_inited = true;
}

static uint64_t *prime_arr = NULL;
static size_t prime_cnt = 0;
static bool primes_inited = false;

static void init_primes(void)
{
    if (primes_inited) return;
    /* Enough to cover the 64th prime (311) */
    const idx_t LIMIT = 2000ULL;
    build_primes_upto(LIMIT, &prime_arr, &prime_cnt);
    primes_inited = true;
}

seq_t a(idx_t n)
{
    init_binom();
    init_primes();

    if (n == 0) return (seq_t)1;            /* row 0, element 1 */

    /* Find the row r (>=1).  For n>0, r = floor_log2(n)+1. */
    idx_t r = 64 - __builtin_clzll(n);      /* r >= 1 for n>0 */
    idx_t start = 1ULL << (r - 1);          /* first index of row r */
    idx_t pos = n - start;                  /* position within the row, 0‑based */

    idx_t N = r - 1;                        /* number of primes smaller than the row’s prime */

    /* Determine the number of other primes k (k = ω−1). */
    uint64_t remaining = pos;
    uint64_t k = 0;
    while (k <= N) {
        uint64_t cnt = C_binom[N][k];
        if (remaining < cnt) break;
        remaining -= cnt;
        ++k;
    }
    uint64_t rank = remaining;              /* 0‑based rank within the k‑segment */

    /* Unrank the combination of size k from N elements
       in the order described: increasing maximal prime,
       then increasing next maximal, etc. */
    unsigned long long comb[65];
    unsigned long long comb_idx = 0;
    uint64_t curRank = rank;
    uint64_t remN = N;
    uint64_t remK = k;

    while (remK > 0) {
        uint64_t start_m = remK - 1;
        uint64_t m;
        for (m = start_m; m < remN; ++m) {
            uint64_t cnt = C_binom[m][remK - 1];
            if (curRank < cnt) break;
            curRank -= cnt;
        }
        comb[comb_idx++] = m;          /* m is the largest chosen index for this level */
        remN = m;                      /* subsequent choices must come from [0 .. m-1] */
        --remK;
    }

    seq_t result = (seq_t)prime_arr[r - 1];   /* largest prime of the row */
    for (unsigned long long i = 0; i < k; ++i) {
        result = seq_mul(result, (seq_t)prime_arr[comb[i]]);
    }
    return result;
}
