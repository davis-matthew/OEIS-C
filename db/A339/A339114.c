/*
 * A339114  Least semiprime whose prime indices sum to n.
 * https://oeis.org/A339114
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least semiprime with prime indices summing to n. For n ≥
 *            2, a(n) = min { p_i * p_j | i + j = n }, where p_k is the k‑th
 *            prime (p_1 = 2)
 * Range:     u8 n <= 13, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto() */

static uint64_t *prime_arr = NULL;   /* 0‑based array of primes */
static size_t   prime_cnt = 0;       /* number of primes stored */

/* Ensure prime_arr contains at least idx_needed primes (1‑based indexing). */
static void
ensure_primes(idx_t idx_needed)
{
    if (prime_cnt >= (size_t)idx_needed)
        return;

    /* Rough upper bound for the idx_needed‑th prime is < 20·idx_needed + 100
       for idx_needed up to at least 1 000 000. */
    uint64_t limit = 20ULL * idx_needed + 100ULL;
    while (1) {
        uint64_t *tmp;
        size_t   cnt;
        build_primes_upto(limit, &tmp, &cnt);  /* primes ≤ limit */

        if (cnt >= (size_t)idx_needed) {
            if (prime_arr)
                free(prime_arr);
            prime_arr = tmp;
            prime_cnt = cnt;
            return;
        }
        free(tmp);
        limit *= 2;   /* expand range */
    }
}

seq_t
a(idx_t n)
{
    if (n < 2)
        return 0;  /* offset is 2; behaviour below offset does not matter */

    /* We need primes up to index n‑1 (largest index in a pair i+j=n). */
    ensure_primes(n - 1);

    seq_t min_val = 0;
    bool   first   = true;

    for (idx_t i = 1; i <= n / 2; ++i) {
        idx_t j = n - i;
        seq_t pi = (seq_t)prime_arr[i - 1]; /* 0‑based array */
        seq_t pj = (seq_t)prime_arr[j - 1];
        seq_t prod = pi * pj;
        if (first || prod < min_val) {
            min_val = prod;
            first   = false;
        }
    }

    return min_val;
}
