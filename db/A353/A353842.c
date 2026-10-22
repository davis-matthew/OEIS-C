/*
 * A353842  Last part of the trajectory of the partition run-sum
 *          transformation of n, using Heinz numbers.
 * https://oeis.org/A353842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = last square‑free member of the run‐sum trajectory of n, *
 *            where the run‑sum transform is applied to the Heinz number of a
 *            partition. * The partition corresponding to a number m is
 *            obtained from the prime * factorisation of m: if m = ∏p_i^{e_i}
 *            then the partition is * (index(p_1) repeated e...
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, get_prime, is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_squarefree.h"

static uint64_t *primes_arr = NULL;   /* array of prime values */
static size_t primes_cnt   = 0;       /* number of primes stored */

/* initialise the prime list (once) */
static void init_primes(void)
{
    if (primes_arr != NULL) return;
    uint64_t limit = 5000000ULL;          /* 5 million gives ~348 k primes */
    build_primes_upto(limit, &primes_arr, &primes_cnt);
}

/* extend the prime list so that it contains all primes ≤ new_limit */
static void extend_primes_upto(uint64_t new_limit)
{
    if (primes_arr == NULL) init_primes();
    if (new_limit <= primes_arr[primes_cnt - 1]) return;

    uint64_t *tmp_arr;
    size_t   tmp_cnt;
    build_primes_upto(new_limit, &tmp_arr, &tmp_cnt);
    free(primes_arr);            /* free old array */
    primes_arr = tmp_arr;
    primes_cnt = tmp_cnt;
}

/* binary search for the 1‑based index of prime p */
static idx_t index_of_prime(uint64_t p)
{
    init_primes();

    size_t lo = 0, hi = primes_cnt;
    while (lo < hi)
    {
        size_t mid = (lo + hi) >> 1;
        if (primes_arr[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < primes_cnt && primes_arr[lo] == p)
        return (idx_t)(lo + 1);

    /* p is larger than any stored prime – extend and try again */
    if (primes_arr[primes_cnt - 1] < p)
    {
        extend_primes_upto(p);
        return index_of_prime(p);   /* recursion safe because p now ≤ last prime */
    }
    return 0;                       /* should never happen for valid input */
}

/* perform one run‑sum transformation on the Heinz number m */
static seq_t run_sum_transform(seq_t m)
{
    uint64_t tmp   = (uint64_t)m;
    uint64_t pf[64], ex[64];
    size_t   fcnt;

    factor_u64(tmp, pf, ex, &fcnt);

    seq_t prod = 1;
    for (size_t i = 0; i < fcnt; ++i)
    {
        idx_t   idx = index_of_prime(pf[i]);          /* index of prime p_i */
        idx_t   new_idx = idx * (idx_t)ex[i];          /* run‑sum result */
        seq_t   prime_val = (seq_t)get_prime(new_idx);
        prod *= prime_val;
    }
    return prod;
}

/* required public function */
seq_t a(idx_t n)
{
    seq_t cur = (seq_t)n;
    if (cur == 1) return 1;    /* 1 is square‑free */

    while (!is_squarefree(cur))
        cur = run_sum_transform(cur);

    return cur;
}
