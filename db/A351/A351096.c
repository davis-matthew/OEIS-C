/*
 * A351096  Semiprimes that are the arithmetic derivative (A003415) of some
 *          number.
 * https://oeis.org/A351096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence computed by enumerating n, finding its arithmetic derivative,
   collecting the derivatives that are semiprimes, sorting and deduplicating.
   The resulting list is stored in a static array to allow O(1) random access. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Compare two seq_t values for qsort. Works for all supported seq_t types. */
static int seq_compare(const void *a, const void *b)
{
    const seq_t *x = (const seq_t *)a;
    const seq_t *y = (const seq_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return  1;
    return 0;
}

/* ------------------------------------------------------------------ */
/* Return true if x is a semiprime (product of exactly two primes,
   allowing equal primes).  primes[] contains all primes up to the sieve
   limit; primes_cnt is its length. */
static bool is_semiprime(seq_t x, const uint32_t *primes, size_t primes_cnt)
{
    if (x < (seq_t)4)              /* 4 = 2² is the smallest semiprime */
        return false;

    int count = 0;
    unsigned long long t = (unsigned long long)x;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        unsigned long long pp = (unsigned long long)p * (unsigned long long)p;
        if (pp > t)
            break;
        while (t % p == 0ULL) {
            t /= p;
            if (++count > 2)
                return false;
        }
    }

    if (t > 1ULL)   /* remaining factor is prime */
        ++count;

    return count == 2;
}

/* ------------------------------------------------------------------ */
/* Global state for the memoized sequence. */
static seq_t *seq   = NULL;          /* array of terms */
static idx_t  seq_len = 0;           /* number of terms */
static bool   seq_initialized = false;

/* ------------------------------------------------------------------ */
/* Build the sequence once on the first call.  B is chosen large enough
   empirically to contain >10 000 terms. */
static void build_sequence(void)
{
    const size_t B = 1200000;   /* upper bound for n */

    /* 1. Sieve of Eratosthenes storing the smallest prime factor. */
    uint32_t *spf  = (uint32_t *)malloc((B + 1) * sizeof(uint32_t));
    uint32_t *primes = (uint32_t *)malloc((B / 10) * sizeof(uint32_t));
    size_t primes_cnt = 0, primes_cap = B / 10;

    for (size_t i = 0; i <= B; ++i)
        spf[i] = (uint32_t)i;

    for (size_t i = 2; i <= B; ++i) {
        if (spf[i] == (uint32_t)i) {                 /* i is prime */
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint32_t *)realloc(primes,
                                              primes_cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt &&
             (unsigned long long)i * primes[j] <= B &&
             primes[j] <= spf[i]; ++j) {
            spf[(unsigned long long)i * primes[j]] = primes[j];
        }
    }

    /* 2. Enumerate n, compute its arithmetic derivative, collect
       semiprime derivatives that fit into seq_t. */
    size_t capacity = 100000;
    seq_t *vals = (seq_t *)malloc(capacity * sizeof(seq_t));
    size_t val_cnt = 0;

    seq_t max_val = (seq_t)~0;          /* maximum value that seq_t can hold */

    for (size_t n = 2; n <= B; ++n) {
        unsigned long long deriv = 0;
        unsigned long long tmp = n;
        while (tmp > 1ULL) {
            unsigned long long p = spf[tmp];
            unsigned long long exp = 0;
            while (tmp % p == 0ULL) {
                tmp /= p;
                ++exp;
            }
            deriv += exp * (unsigned long long)(n / p);
        }

        if (deriv > (unsigned long long)max_val)
            continue;                      /* value would overflow seq_t */

        seq_t val = (seq_t)deriv;
        if (is_semiprime(val, primes, primes_cnt)) {
            if (val_cnt == capacity) {
                capacity <<= 1;
                vals = (seq_t *)realloc(vals, capacity * sizeof(seq_t));
            }
            vals[val_cnt++] = val;
        }
    }

    /* 3. Sort and deduplicate the collected values. */
    qsort(vals, val_cnt, sizeof(seq_t), seq_compare);

    size_t uniq = 0;
    for (size_t i = 0; i < val_cnt; ++i) {
        if (i == 0 || vals[i] != vals[i - 1]) {
            vals[uniq++] = vals[i];
        }
    }

    seq = (seq_t *)realloc(vals, uniq * sizeof(seq_t));
    seq_len = (idx_t)uniq;

    /* Clean up temporary arrays. */
    free(spf);
    free(primes);
}

/* ------------------------------------------------------------------ */
/* Public function required by the exercise. */
seq_t a(idx_t n)
{
    if (!seq_initialized) {
        build_sequence();
        seq_initialized = true;
    }
    if (n == 0 || (size_t)n > (size_t)seq_len)
        return (seq_t)0;          /* undefined: return 0 */
    return seq[(size_t)n - 1];
}
