/*
 * A241914  After a(1)=0, numbers 0 .. A061395(n)-1, followed by numbers 0 ..
 *          A061395(n+1)-1, etc.
 * https://oeis.org/A241914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 9819, u16 n <= 9819, u32 n <= 9819, u64 n <= 9819,
 *            u128 n <= 9819, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A241914
 *
 *  Sequence is obtained by appending, for each positive integer r,
 *  the numbers 0,1,…,A061395(r)-1, where A061395(r) is the rank
 *  (starting at 1) of the largest prime factor of r.
 *  a(n) is the value of the n‑th element of this concatenation.
 *  The implementation builds rows lazily and caches
 *  cumulative row lengths; it works for all allowed idx_t / seq_t
 *  types and satisfies the required constraints.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Static state ********************************************************************/

/* cumulative row lengths: cum_len[0] = 0, cum_len[i] = sum_{k=1..i} length(k) */
static idx_t *cum_len = NULL;
static size_t   cum_len_cap = 0;
static size_t   cum_len_len = 0;          /* number of stored entries */

/* current highest processed row */
static idx_t   next_row = 0;

/* prime list for factorisation and lookup of prime indices */
static uint32_t *primes = NULL;
static size_t   primes_cap = 0;
static size_t   primes_count = 0;
static idx_t   primes_limit = 1;          /* largest number whose primes are known */

/* flags */
static bool    initialized = false;

/* Forward declarations of helper functions */
static void      init_state(void);
static void      ensure_primes_up_to(idx_t n);
static idx_t    largest_prime_factor_index(idx_t n);
static void      extend_rows_until(idx_t target_n);

/* =============================================================================== */
/*  Helper functions */

/* initialise all static structures */
static void init_state(void)
{
    if (initialized) return;

    /* allocate minimal capacity for cumulative lengths */
    cum_len_cap = 16;
    cum_len = (idx_t*)calloc(cum_len_cap, sizeof(idx_t));
    if (!cum_len) exit(EXIT_FAILURE);
    cum_len[0] = 0;
    cum_len_len = 1;            /* only the sentinel 0 is stored */

    /* initial capacity for prime list */
    primes_cap = 64;
    primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    primes_count = 0;
    primes_limit = 1;           /* nothing known above 1 */

    next_row = 0;
    initialized = true;
}

/* extend the list of primes up to (and including) n, if necessary */
static void ensure_primes_up_to(idx_t n)
{
    if (n <= primes_limit) return;

    for (idx_t num = primes_limit + 1; num <= n; ++num) {
        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i) {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > num) break;
            if (num % p == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            if (primes_count == primes_cap) {
                primes_cap = primes_cap ? primes_cap * 2 : 64;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!primes) exit(EXIT_FAILURE);
            }
            primes[primes_count++] = (uint32_t)num;
        }
    }
    primes_limit = n;
}

/* return the rank (starting at 1) of the largest prime factor of n.
 * For n = 1 the function returns 1 by convention (row 1 has length 1). */
static idx_t largest_prime_factor_index(idx_t n)
{
    if (n <= 1) return 1;   /* special convention for row 1 */

    idx_t remaining = n;
    uint32_t largest = 1;

    for (size_t i = 0; i < primes_count && (uint64_t)primes[i] * (uint64_t)primes[i] <= remaining; ++i) {
        uint32_t p = primes[i];
        while (remaining % p == 0) {
            largest = p;
            remaining /= p;
        }
    }
    if (remaining > 1) largest = (uint32_t)remaining;

    /* binary search for the index of 'largest' in the prime list */
    size_t lo = 0, hi = primes_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] >= largest) hi = mid;
        else                          lo = mid + 1;
    }
    return (idx_t)(lo + 1);   /* 1‑based index */
}

/* extend rows until the cumulative length reaches at least target_n */
static void extend_rows_until(idx_t target_n)
{
    while (cum_len[cum_len_len - 1] < target_n) {
        ++next_row;

        idx_t row_len;
        if (next_row == 1) {
            row_len = 1;           /* row 1 has one element (0) */
        } else {
            ensure_primes_up_to(next_row);
            row_len = largest_prime_factor_index(next_row);
        }

        /* grow the cum_len array if necessary */
        if (cum_len_len + 1 > cum_len_cap) {
            cum_len_cap = cum_len_cap ? cum_len_cap * 2 : 16;
            cum_len = (idx_t*)realloc(cum_len, cum_len_cap * sizeof(idx_t));
            if (!cum_len) exit(EXIT_FAILURE);
        }

        cum_len[cum_len_len] = cum_len[cum_len_len - 1] + row_len;
        ++cum_len_len;
    }
}

/* =============================================================================== */
/*  Main sequence function */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;    /* undefined for n < 1 – return 0 */

    init_state();

    /* generate enough rows to cover index n */
    extend_rows_until(n);

    /* binary search to find the row that contains index n */
    size_t low = 1;
    size_t high = cum_len_len - 1;
    while (low < high) {
        size_t mid = (low + high) >> 1;
        if (cum_len[mid] >= n) high = mid;
        else                   low = mid + 1;
    }
    size_t row = low;

    /* offset inside the row (0‑based) */
    seq_t offset = (seq_t)(n - cum_len[row - 1] - (idx_t)1);
    return offset;
}
