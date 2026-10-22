/*
 * A363619  Weighted alternating sum of the multiset of prime indices of n.
 * https://oeis.org/A363619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 68, i16 n <= 68, i32 n <= 68, i64 n <= 68, i128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Weighted alternating sum of the multiset of prime indices of n
   (OEIS A363619).  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Static cache of primes.  */
static uint64_t *primes     = NULL;   /* primes[0 .. prime_cnt-1]   */
static size_t   prime_cnt  = 0;       /* number of primes stored   */
static idx_t    prime_lim  = 0;       /* largest prime at which
                                         the table has been built  */

/* Ensure that the global prime table contains all primes ≤ limit. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= prime_lim) return;

    /* Allocate boolean sieve.  */
    bool *sieve = (bool*)calloc((size_t)limit + 1, sizeof(bool));
    if (!sieve) abort();

    /* Mark primes.  */
    for (idx_t i = 2; i <= limit; ++i) sieve[i] = true;
    for (idx_t p = 2; (idx_t)p * p <= limit; ++p) {
        if (sieve[p]) {
            for (idx_t j = p * p; j <= limit; j += p) sieve[j] = false;
        }
    }

    /* Count primes and rebuild the array.  */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) ++cnt;

    uint64_t *new_primes = (uint64_t*)realloc(primes, cnt * sizeof(uint64_t));
    if (!new_primes) abort();
    primes = new_primes;

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) primes[idx++] = (uint64_t)i;

    prime_cnt = cnt;
    prime_lim = limit;

    free(sieve);
}

/* Main sequence function.  */
seq_t a(idx_t n)
{
    if (n < 2) return 0;             /* a(1) = 0 */

    /* Make sure all primes ≤ n are known.  */
    ensure_primes_upto(n);

    seq_t sum  = 0;
    seq_t pos  = 1;                  /* 1‑based position in the weighted sum */
    uint64_t rem = (uint64_t)n;      /* remaining part to factor */

    /* Process factors ≤ sqrt(n) in ascending order.  */
    for (size_t k = 0; k < prime_cnt && primes[k] <= rem; ++k) {
        uint64_t p = primes[k];
        if (rem % p == 0) {
            size_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            seq_t idx = (seq_t)(k + 1);  /* prime index (1‑based) */
            for (size_t j = 0; j < exp; ++j) {
                seq_t term = idx * pos;
                if (pos & 1) sum += term;   /* odd position → plus   */
                else       sum -= term;     /* even position → minus */
                ++pos;
            }
        }
    }

    /* Any remaining factor is a prime > sqrt(original n).  */
    if (rem > 1) {
        ensure_primes_upto(rem);          /* it will be in the table now */
        for (size_t k = 0; k < prime_cnt; ++k) {
            if (primes[k] == rem) {
                seq_t idx = (seq_t)(k + 1);
                seq_t term = idx * pos;
                if (pos & 1) sum += term;
                else        sum -= term;
                break;
            }
        }
    }

    return sum;
}
