/*
 * A358137  Heinz number of the partial sums of the prime indices of n.
 * https://oeis.org/A358137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π prime(S_k) where the S_k are the partial sums of the
 *            prime indices of the prime factorisation of n (counted with
 *            multiplicity and in ascending order of primes)
 * Range:     u8 n <= 23, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   build_primes, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies from the utils library */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes.h"
#include "../utils/get_prime.h"

/* Globals to hold the prime list and its count */
static idx_t *prime_list   = NULL;   /* primes <= LIMIT (2‑based indices) */
static size_t prime_cnt    = 0;

/* Initialise primes once */
static void
init_prime_table(void)
{
    if (prime_list != NULL)
        return;                          /* already initialised */

    /* This limit covers all primes needed for n <= 10000
       (prime indices up to about 1229) and their partial sums
       stay far below it. */
    const idx_t LIMIT = 20000;
    build_primes(LIMIT, &prime_list, &prime_cnt);
}

/* Binary search for the 1‑based index of prime p in prime_list. */
static inline idx_t
prime_index(idx_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] == p)
            return (idx_t)(mid + 1);     /* 1‑based index */
        if (prime_list[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return 0;   /* not found – should not happen for valid input */
}

/* Factorise n into primes using the pre‑computed prime list. */
static void
factorise(idx_t n, idx_t *primes, idx_t *exps, size_t *cnt)
{
    size_t c = 0;
    idx_t t = n;
    for (size_t i = 0; i < prime_cnt && (idx_t)prime_list[i] * prime_list[i] <= t; ++i) {
        idx_t p = prime_list[i];
        if (t % p == 0) {
            idx_t e = 0;
            while (t % p == 0) {
                t /= p;
                ++e;
            }
            primes[c] = p;
            exps[c]   = e;
            ++c;
        }
    }
    if (t > 1) {
        primes[c] = t;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset starts at 1; behaviour below is irrelevant */
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    init_prime_table();

    /* At most 6 distinct primes for n <= 10000 */
    idx_t fp[8], ex[8];
    size_t fp_cnt = 0;
    factorise(n, fp, ex, &fp_cnt);

    seq_t prod = (seq_t)1;
    idx_t cum   = 0;
    for (size_t i = 0; i < fp_cnt; ++i) {
        idx_t k = prime_index(fp[i]);      /* index of the prime itself */
        for (idx_t j = 0; j < ex[i]; ++j) {
            cum += k;
            prod = prod * (seq_t)get_prime(cum);
        }
    }
    return prod;
}
