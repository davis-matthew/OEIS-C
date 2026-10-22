/*
 * A302643  Number of partitions of n into two parts such that the positive
 *          difference of the parts is a squarefree semiprime.
 * https://oeis.org/A302643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = number of partitions of n into two parts such that
 *  the positive difference of the parts is a squarefree semiprime.
 *  Computed by counting all squarefree semiprime differences <= n-2.
 *  The implementation builds a prefix table of such differences
 *  up to the requested limit. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* static cache for prefix counts of squarefree semiprimes (even/odd) */
static uint64_t *pref_even = NULL;
static uint64_t *pref_odd  = NULL;
static idx_t     cached_limit = 0;  /* largest limit that has been cached */

/* enlarge the cache so that it covers all numbers ≤ new_limit */
static void ensure_cache(idx_t new_limit)
{
    if (new_limit <= cached_limit) return;

    /* free old arrays if they exist */
    if (pref_even) free(pref_even);
    if (pref_odd)  free(pref_odd);

    /* allocate prefix arrays with an extra slot for index 0 */
    pref_even = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
    pref_odd  = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));

    /* initialize the zero-th element (no numbers ≤ 0) */
    pref_even[0] = 0;
    pref_odd[0]  = 0;

    /* sieve to find primes up to new_limit */
    bool *is_prime = (bool*)calloc(new_limit + 1, sizeof(bool));
    for (idx_t i = 2; i <= new_limit; ++i) is_prime[i] = true;
    if (new_limit >= 0) is_prime[0] = false;
    if (new_limit >= 1) is_prime[1] = false;
    for (idx_t p = 2; p * p <= new_limit; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t q = p * p; q <= new_limit; q += p) is_prime[q] = false;
    }

    /* collect primes into an array */
    uint64_t *primes = (uint64_t*)malloc((new_limit + 1) * sizeof(uint64_t));
    size_t prime_cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i) {
        if (is_prime[i]) primes[prime_cnt++] = i;
    }

    /* mark squarefree semiprimes (product of two distinct primes) */
    bool *semip = (bool*)calloc(new_limit + 1, sizeof(bool));
    for (size_t a = 0; a < prime_cnt; ++a) {
        uint64_t pa = primes[a];
        if (pa > new_limit / 2) break;            /* pa*pb > new_limit once pa > new_limit/2 */
        for (size_t b = a + 1; b < prime_cnt; ++b) {
            uint64_t pb  = primes[b];
            if (pa > new_limit / pb) break;        /* early stop for overflow/size */
            uint64_t prod = pa * pb;
            if (prod > new_limit) break;
            semip[prod] = true;
        }
    }

    /* build prefix counts for even and odd squarefree semiprimes */
    uint64_t even_sum = 0, odd_sum = 0;
    for (idx_t k = 1; k <= new_limit; ++k) {
        if (semip[k]) {
            if ((k & 1) == 0) even_sum++;
            else              odd_sum++;
        }
        pref_even[k] = even_sum;
        pref_odd[k]  = odd_sum;
    }

    /* clean up temporary allocations */
    free(is_prime);
    free(primes);
    free(semip);

    cached_limit = new_limit;
}

/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* a(1) and a(2) are defined as 0 */
    if (n <= 2) return 0;

    /* the largest difference we need to consider is n-2 */
    idx_t limit = n - 2;
    ensure_cache(limit);

    /* if n is even we count even differences, otherwise odd */
    if ((n & 1) == 0)
        return (seq_t)pref_even[limit];
    else
        return (seq_t)pref_odd[limit];
}
