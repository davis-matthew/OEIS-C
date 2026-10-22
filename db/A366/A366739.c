/*
 * A366739  Number of distinct semi-sums of the multiset of prime indices of
 *          n. Number of distinct sums of prime indices of semiprime divisors
 *          of n (counted by A086971).
 * https://oeis.org/A366739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8428, u16 n <= 8428, u32 n <= 8428, u64 n <= 8428,
 *            u128 n <= 8428, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the number of distinct sums of two prime indices obtained from
   the prime factorisation of n (with multiplicities).  The prime index of
   a prime p is the position of p in the increasing sequence of primes,
   e.g. 2→1, 3→2, 5→3, ….
   For each prime factor p^e of n we insert the index of p exactly e
   times into a multiset.  A semi‑sum is the sum of any 2‑element
   sub‑multiset of this multiset.  The result is the number of distinct
   values that can occur as such a sum.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/*  Prime table – sieve up to a fixed limit.  The limit is chosen big
 *  enough to cover all prime factors that occur for n up to about 10⁴.
 *  The table is built on first use and reused for subsequent calls.
 *  --------------------------------------------------------------------- */
#define PRIMES_LIMIT 200000           /* > max prime factor of 10⁴ */

static uint64_t *prime_tbl = NULL;    /* array of primes (1‑based positions) */
static size_t     prime_cnt = 0;      /* number of primes */

/* Build the prime list once; safe for both C and C++ compilers. */
static void build_prime_table(void)
{
    if (prime_tbl != NULL)            /* already built */
        return;

    bool *sieve = (bool *)calloc(PRIMES_LIMIT + 1, sizeof(bool));
    if (!sieve)                      /* allocation failure – give up */
        return;

    size_t i, j;
    for (i = 0; i <= PRIMES_LIMIT; ++i)
        sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (i = 2; i * i <= PRIMES_LIMIT; ++i)
        if (sieve[i])
            for (j = i * i; j <= PRIMES_LIMIT; j += i)
                sieve[j] = false;

    /* Count primes */
    for (i = 2; i <= PRIMES_LIMIT; ++i)
        if (sieve[i])
            ++prime_cnt;

    prime_tbl = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
    if (!prime_tbl) {
        free(sieve);
        return;
    }

    size_t k = 0;
    for (i = 2; i <= PRIMES_LIMIT; ++i)
        if (sieve[i])
            prime_tbl[k++] = i;

    free(sieve);
}

/* Return the 1‑based index of prime p (assumes p is prime and ≤ PRIMES_LIMIT). */
static uint64_t prime_index(uint64_t p)
{
    /* Binary search in prime_tbl */
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_tbl[mid] <= p)
            lo = mid + 1;
        else
            hi = mid;
    }
    /* lo is the number of primes ≤ p, i.e. its index */
    return (uint64_t)lo;
}

/* --------------------------------------------------------------------- */
/*  Main sequence function.
 *  --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)                     /* undefined, but return 0 */
        return (seq_t)0;

    /* Ensure prime table is ready */
    build_prime_table();
    if (prime_tbl == NULL)         /* build failed; give 0 */
        return (seq_t)0;

    /* Factor n and build the multiset of prime indices */
    uint64_t temp = (uint64_t)n;
    uint64_t idxs[64];              /* enough for all exponents of n ≤ 10⁴   */
    size_t sz = 0;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            size_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            uint64_t idx = prime_index(p);
            for (size_t k = 0; k < cnt; ++k)
                idxs[sz++] = idx;
        }
    }
    if (temp > 1) {                 /* remaining prime factor */
        uint64_t idx = prime_index(temp);
        idxs[sz++] = idx;
    }

    if (sz < 2)
        return (seq_t)0;            /* cannot form a 2‑element multiset */

    /* Determine maximum possible sum to size the bitmap */
    uint64_t max_idx = 0;
    for (size_t i = 0; i < sz; ++i)
        if (idxs[i] > max_idx)
            max_idx = idxs[i];
    size_t maxsum = (size_t)(max_idx * 2);   /* inclusive upper bound */

    bool *seen = (bool *)calloc(maxsum + 1, sizeof(bool));
    if (!seen)
        return (seq_t)0;            /* allocation failure – return 0 */

    seq_t count = (seq_t)0;
    for (size_t i = 0; i < sz; ++i) {
        for (size_t j = i + 1; j < sz; ++j) {
            size_t sum = (size_t)(idxs[i] + idxs[j]);
            if (!seen[sum]) {
                seen[sum] = true;
                ++count;
            }
        }
    }

    free(seen);
    return count;
}
