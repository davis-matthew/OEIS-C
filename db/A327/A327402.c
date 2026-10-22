/*
 * A327402  Quotient of n over the maximum stable divisor of n.
 * https://oeis.org/A327402
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / (maximum stable divisor of n)
 * Range:     u8 n <= 704, u16 n <= 9815, u32 n <= 9815, u64 n <= 9815,
 *            u128 n <= 9815, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------------------------------------------------------------- */
/* Simple prime sieve (maximum limit 70000 is sufficient for n ≤ 10 000)
/* --------------------------------------------------------------------- */
#define SIEVE_LIMIT 70000

static bool primes_sieved = false;          /* flag for one‑time init     */
static uint64_t primes_array[SIEVE_LIMIT + 1];
static size_t primes_cnt = 0;

/* Build the list of primes up to SIEVE_LIMIT (lazy, thread‑unsafe). */
static void init_prime_table(void)
{
    if (primes_sieved) return;

    /* Allocate a temporary boolean array (explicit cast for C++). */
    bool *is_comp = (bool *)malloc(SIEVE_LIMIT + 1);
    if (!is_comp) return;            /* allocation failure – fall back to
                                         unsieved factorisation (rare). */

    for (size_t i = 0; i <= SIEVE_LIMIT; ++i) is_comp[i] = false;

    for (size_t i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_comp[j] = true;
        }
    }

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i)
        if (!is_comp[i]) primes_array[primes_cnt++] = i;

    free(is_comp);
    primes_sieved = true;
}

/* --------------------------------------------------------------------- */
/* Locate the 1‑based index of a prime p in primes_array.  Returns 0
   if p is not found – this can only happen if p > SIEVE_LIMIT.      */
/* --------------------------------------------------------------------- */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes_array[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    if (lo < primes_cnt && primes_array[lo] == p)
        return lo + 1;      /* 1‑based index */
    return 0;
}

/* --------------------------------------------------------------------- */
/* Factorise n into distinct primes and their exponents.  The arrays
   pr[] and ex[] must be large enough (at least 16 elements is safe).   */
/* --------------------------------------------------------------------- */
static void factor_uint64(uint64_t n, uint64_t *pr, int *ex, size_t *cnt)
{
    size_t k = 0, i = 0;
    uint64_t tmp = n;

    while (i < primes_cnt && primes_array[i] * primes_array[i] <= tmp) {
        uint64_t p = primes_array[i];
        if (tmp % p == 0) {
            int e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            pr[k] = p;
            ex[k] = e;
            ++k;
        }
        ++i;
    }

    if (tmp > 1) {          /* remaining prime factor exceeds the sieve */
        pr[k] = tmp;
        ex[k] = 1;
        ++k;
    }
    *cnt = k;
}

/* --------------------------------------------------------------------- */
/* Main function requested by the build system.                          */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* should never be called for 0 */
    init_prime_table();

    /* Factorise n.  16 distinct prime factors are enough for all n ≤ 10 000. */
    uint64_t pr[16];
    int ex[16];
    size_t fac_cnt = 0;
    factor_uint64((uint64_t)n, pr, ex, &fac_cnt);

    /* Pre‑compute prime indices and the full power pi^ei. */
    size_t prime_idx[16];
    seq_t prime_pow[16];
    for (size_t i = 0; i < fac_cnt; ++i) {
        prime_idx[i] = prime_index(pr[i]);             /* 1‑based index */
        seq_t p = (seq_t)pr[i];
        seq_t val = 1;
        for (int t = 0; t < ex[i]; ++t) val *= p;
        prime_pow[i] = val;
    }

    /* Enumerate all non‑empty subsets of distinct primes and keep the
       largest product that respects the pairwise indivisibility condition
       on their indices. */
    seq_t best = 1;                             /* product of the subset */
    size_t total_masks = (size_t)1 << fac_cnt;

    for (size_t mask = 1; mask < total_masks; ++mask) {
        bool ok = true;
        for (size_t i = 0; i < fac_cnt && ok; ++i) {
            if (!(mask & ((size_t)1 << i))) continue;
            for (size_t j = i + 1; j < fac_cnt; ++j) {
                if (!(mask & ((size_t)1 << j))) continue;
                size_t aidx = prime_idx[i];
                size_t bidx = prime_idx[j];
                if (aidx % bidx == 0 || bidx % aidx == 0) {
                    ok = false;
                    break;
                }
            }
        }
        if (!ok) continue;

        seq_t prod = 1;
        for (size_t i = 0; i < fac_cnt; ++i)
            if (mask & ((size_t)1 << i))
                prod *= prime_pow[i];

        if (prod > best) best = prod;
    }

    seq_t n_seq = (seq_t)n;
    return n_seq / best;
}
