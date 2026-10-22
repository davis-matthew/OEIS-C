/*
 * A374248  Sum of prime indices of n (with multiplicity) minus the greatest
 *          possible sum of run-compression of a permutation of the prime
 *          indices of n.
 * https://oeis.org/A374248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A056239(n) - A373956(n)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where A056239(n) = sum of prime indices of n (with multiplicity) */
/* and A373956(n) = maximum possible sum of run‑compression of a permutation */
/* of that multiset of prime indices. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/* Simple prime sieve up to MAX_PRIME_LIMIT (chosen large enough for the */
/* expected n <= 10000).                                               */
/* --------------------------------------------------------------------- */
#define MAX_PRIME_LIMIT 100000 /* safe upper bound for n ≈ 10⁴ */

static bool primes_init = false;
static size_t prime_cnt = 0;
static uint32_t primes[20000]; /* enough for primes up to 100000 */

static void init_primes(void)
{
    if (primes_init) return;
    uint8_t is_prime[MAX_PRIME_LIMIT + 1];
    for (size_t i = 0; i <= MAX_PRIME_LIMIT; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;
    for (size_t i = 2; i * i <= MAX_PRIME_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= MAX_PRIME_LIMIT; j += i)
                is_prime[j] = 0;
    for (size_t i = 2; i <= MAX_PRIME_LIMIT; ++i)
        if (is_prime[i])
            primes[prime_cnt++] = (uint32_t)i;
    primes_init = true;
}

/* binary search for prime index (1‑based). Returns 0 if not found. */
static idx_t prime_index(uint32_t p)
{
    size_t lo = 0, hi = prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    if (lo < prime_cnt && primes[lo] == p) return (idx_t)lo + 1; /* 1‑based */
    return 0; /* shouldn't happen for valid p */
}

/* --------------------------------------------------------------------- */
/* Main function required by the build.                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 1) return 0; /* a(1) = 0 */

    init_primes(); /* ensure primes are ready */

    /* factor n -------------------------------------------------------- */
    uint64_t tmp = (uint64_t)n;
    idx_t total_factors = 0;    /* Ω(n) */
    seq_t sum_indices = 0;      /* A056239(n) */
    /* we store at most 20 distinct prime factors for n <= 10⁴ */
    idx_t idxs[20];
    idx_t cnts[20];
    size_t distinct = 0;

    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= tmp; ++i) {
        uint32_t p = primes[i];
        if (tmp % p == 0) {
            idx_t exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            idx_t m = prime_index(p); /* index of prime p */
            if (m == 0) continue; /* safety */
            idxs[distinct] = m;
            cnts[distinct] = exp;
            ++distinct;
            total_factors += exp;
            sum_indices += (seq_t)m * exp;
        }
    }
    if (tmp > 1) {
        /* remaining prime factor > sqrt(original n) */
        uint32_t p = (uint32_t)tmp;
        idx_t m = prime_index(p);
        if (m != 0) {
            idxs[distinct] = m;
            cnts[distinct] = 1;
            ++distinct;
            ++total_factors;
            sum_indices += (seq_t)m;
        }
    }

    if (total_factors == 0) return 0; /* should only happen for n=1 */

    /* compute maximal run‑compression sum -------------------------------- */
    seq_t sum_runs = 0;
    for (size_t i = 0; i < distinct; ++i) {
        idx_t m = idxs[i];
        idx_t c = cnts[i];
        idx_t runs = c;
        if (runs > total_factors - c + 1)
            runs = total_factors - c + 1;
        sum_runs += (seq_t)m * runs;
    }

    return sum_indices - sum_runs;
}
