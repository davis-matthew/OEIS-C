/*
 * A293909  Number of Goldbach partitions (p,q) of 2n, p <= q, such that both
 *          2n-2 and 2n+2 have a Goldbach partition with a greater difference
 *          between its prime parts than q-p.
 * https://oeis.org/A293909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 209, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A293909:
   Count of Goldbach partitions (p,q) of 2n, p ≤ q, such that the maximum
   prime‑difference among Goldbach partitions of 2n−2 and 2n+2 is larger
   than q−p.  Computed by sieving primes up to 2n+2. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Forward declaration required by the build */
seq_t a(idx_t n);

/* ----- Global data for the prime sieve ----- */
static idx_t          sieve_limit = 0;   /* largest integer that has been sieved */
static bool          *is_prime    = NULL; /* primality flag array */
static uint64_t      *primes      = NULL; /* list of primes */
static size_t          prime_cnt   = 0;   /* number of primes stored */

/* ----- Helper: extend the sieve up to 'limit' ----- */
static void init_sieve(idx_t limit)
{
    if (limit <= sieve_limit) return;       /* nothing to do */

    /* free old data before allocating new arrays */
    free(is_prime);
    free(primes);

    /* Allocate primality flag array */
    size_t sz = (size_t)(limit + 1);
    is_prime = (bool *)malloc(sz * sizeof(bool));
    if (!is_prime) abort();

    /* Initialise all to true, then mark 0 and 1 as composite */
    for (size_t i = 0; i < sz; ++i) is_prime[i] = true;
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;

    /* Eratosthenes sieve */
    for (idx_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }
    }

    /* Build the list of primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
    uint64_t *tmp = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!tmp) abort();
    size_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i]) tmp[k++] = (uint64_t)i;
    primes    = tmp;
    prime_cnt = cnt;

    sieve_limit = limit;
}

/* ----- Helper: maximum Goldbach difference for an even number e ----- */
static seq_t max_goldbach_diff(idx_t e)
{
    if (e < 4) return 0;                /* no Goldbach partitions exist */

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > e / 2) break;          /* p must be ≤ e/2 to keep q ≥ p */
        idx_t q = e - p;
        if (q <= sieve_limit && is_prime[q]) {
            /* Difference q−p equals e−2p */
            return (seq_t)(q - p);
        }
    }
    return 0;                          /* should never happen for e≥4 */
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* offset is 1 */

    /* Ensure primes up to 2n+2 are available */
    idx_t needed_limit = 2 * n + 2;
    init_sieve(needed_limit);

    /* Compute threshold: the smaller of the two maximum differences */
    seq_t diff_minus = max_goldbach_diff(2 * n - 2);
    seq_t diff_plus  = max_goldbach_diff(2 * n + 2);
    seq_t threshold  = diff_minus < diff_plus ? diff_minus : diff_plus;

    /* Count qualifying Goldbach partitions of 2n */
    seq_t count = 0;
    idx_t two_n = 2 * n;

    for (size_t i = 0; i < prime_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n) break;               /* p <= n ensures p <= q */
        idx_t q = two_n - p;
        if (q <= sieve_limit && is_prime[q]) {
            seq_t diff = (seq_t)(q - p); /* q-p == 2n-2p */
            if (diff < threshold) ++count;
        }
    }

    return count;
}
