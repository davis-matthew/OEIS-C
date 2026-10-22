/*
 * A363681  Sphenic numbers sandwiched between two squarefree semiprimes.
 * https://oeis.org/A363681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Sequence A363681 - sphenic numbers sandwiched between two
 *            squarefree semiprimes
 * Range:     u8 n <= 1, u16 n <= 719, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t *spf = NULL;          /* smallest prime factor table */
static idx_t spf_limit = 0;        /* size of SPF table */
static seq_t seq_arr[20000];       /* preallocated array of sequence terms */
static size_t seq_cnt = 0;         /* number of terms found so far */
static idx_t checked_upto = 2;     /* next integer to test */
static bool initialized = false;

/* Generate smallest prime factor table up to `limit`. */
static void init_sieve(idx_t limit)
{
    spf_limit = limit;
    spf = (idx_t *)malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf) return;                /* allocation failure – very unlikely */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {           /* prime */
            spf[i] = i;
            if (i > limit / i) continue;  /* avoid overflow of i*i */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
}

/* Count distinct prime factors of `n` while checking squarefree.
   If the count equals `target` return that count, otherwise -1. */
static int count_distinct_squarefree(idx_t n, int target)
{
    int cnt = 0;
    while (n > 1) {
        idx_t p = spf[n];
        int exp = 0;
        while (n % p == 0) {
            n /= p;
            exp++;
            if (exp > 1) return -1;   /* not squarefree */
        }
        cnt++;
        if (cnt > target) return -1;   /* too many distinct primes */
    }
    return (cnt == target) ? cnt : -1;
}

static bool is_sphenic(idx_t n)    { return count_distinct_squarefree(n, 3) == 3; }
static bool is_sqfree_semiprime(idx_t n) { return count_distinct_squarefree(n, 2) == 2; }

/* Initialize static data – called once per program lifetime. */
static void initialize(void)
{
    if (initialized) return;
    initialized = true;

    /* Upper bound large enough to contain more than 10000 terms. */
    init_sieve(2000000 + 2);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined for n<1 */

    initialize();

    /* Generate terms until we have at least n of them. */
    while (seq_cnt < n) {
        if (checked_upto >= spf_limit - 1) {
            /* Fallback – unlikely for required n. */
            return 0;
        }
        idx_t m = checked_upto++;
        if (is_sphenic(m) &&
            is_sqfree_semiprime(m - 1) &&
            is_sqfree_semiprime(m + 1)) {

            if (seq_cnt < 20000) {
                seq_arr[seq_cnt++] = (seq_t)m;
            } else {
                /* Sequence is too long; not expected for n up to 10000. */
                return 0;
            }
        }
    }

    return seq_arr[n - 1];
}
