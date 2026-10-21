/*
 * A244578  Sum of all aliquot divisors of all positive integers <= prime(n).
 * https://oeis.org/A244578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{prime(n)} (sigma(k) - k)
 * Range:     u8 n <= 10, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(k) is the sum of all divisors of k.  */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

/* static state */
static uint64_t *primes = NULL;   /* primes[0] is the 1st prime          */
static size_t prime_cnt = 0;      /* number of primes stored             */
static seq_t *sigma = NULL;       /* sigma[i] = sum of divisors of i     */
static seq_t *pref  = NULL;       /* pref[i]  = sum_{k=1}^i sigma(k)    */
static idx_t max_limit = 0;       /* current maximum index precomputed   */
static bool initialized = false;  /* initialise once                      */

/* simple sieve that stores all primes <= limit           */
static void sieve_primes_upto(idx_t limit, uint64_t **out, size_t *cnt) {
    size_t sz = (size_t)limit + 1;
    bool *is_composite = (bool*)calloc(sz, sizeof(bool));
    if (!is_composite) return;
    uint64_t *arr = NULL;
    size_t alloc = 0, used = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_composite[(size_t)i]) {
            if (used == alloc) {
                alloc = alloc ? alloc*2 : 256;
                uint64_t *tmp = (uint64_t*)realloc(arr, alloc * sizeof(uint64_t));
                if (!tmp) { free(is_composite); free(arr); return; }
                arr = tmp;
            }
            arr[used++] = (uint64_t)i;
            for (idx_t j = i * i; j <= limit; j += i) is_composite[(size_t)j] = true;
        }
    }
    free(is_composite);
    *out = arr;
    *cnt = used;
}

/* Initialise global tables up to a sufficient limit. */
static void init(void) {
    if (initialized) return;
    /* Ensure we have at least prime(10000) ≈ 104729. 200000 is safe. */
    idx_t limit = 200000;
    sieve_primes_upto(limit, &primes, &prime_cnt);
    if (prime_cnt == 0) return;          /* safety, should never happen */
    max_limit = limit;
    /* allocate sigma array */
    sigma = (seq_t*)malloc((size_t)(max_limit + 1) * sizeof(seq_t));
    pref  = (seq_t*)malloc((size_t)(max_limit + 1) * sizeof(seq_t));
    if (!sigma || !pref) return;
    sigma[0] = 0;
    sigma[1] = 1;
    for (idx_t i = 2; i <= max_limit; ++i) sigma[i] = 1;
    /* add proper divisors (including i itself) */
    for (idx_t d = 2; d <= max_limit; ++d) {
        for (idx_t m = d; m <= max_limit; m += d) {
            sigma[m] += (seq_t)d;
        }
    }
    /* build prefix sums */
    pref[0] = 0;
    for (idx_t i = 1; i <= max_limit; ++i) {
        pref[i] = pref[i-1] + sigma[i];
    }
    initialized = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    /* initialise on first call */
    if (!initialized) init();
    if (n > prime_cnt) return 0;          /* n too large for precomputed primes */
    uint64_t p = primes[n-1];             /* prime(n) */
    seq_t pseq = (seq_t)p;
    seq_t sum_sigma = pref[p];
    seq_t sum_k     = pseq * (pseq + 1) / 2;
    return sum_sigma - sum_k;
}
