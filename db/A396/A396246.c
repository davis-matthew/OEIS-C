/*
 * A396246  Triangle read by rows: T(n,m) is the number of ways to fill an m X
 *          n grid by using the numbers 1, 2, ..., m*n exactly once, so that 1
 *          is the only entry which is the smallest both in its row and in its
 *          column, 1 <= m <= n.
 * https://oeis.org/A396246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,m) = (m*n)! * m! * n! / (m+n-1)!
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(idx_t n) returns the n‑th element of the
   triangle read by rows, where n starts at 1. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Maximum value of mn = m*n that can occur for
   n (index) up to about 10 000.  For such n we have
   m <= r <= 141, so mn <= 141*141 = 19881.  Work
   comfortably up to 20000. */
static const idx_t MAX_PRIME = 20000;

/* Prime table – built once */
static bool primes_done = false;
static idx_t *primes = NULL;
static size_t prime_cnt = 0;

/* Build list of primes up to limit (inclusive) using a
   simple sieve.  The function is only called once. */
static void build_primes_upto(idx_t limit)
{
    if (primes_done) return;

    char *is_prime = (char*)malloc((limit + 1) * sizeof(char));
    if (!is_prime) return;          /* memory failure */

    for (idx_t i = 0; i <= limit; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t i = 2; i * i <= limit; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;

    primes = (idx_t*)malloc(((size_t)limit + 1) * sizeof(idx_t));
    if (!primes) { free(is_prime); return; }
    prime_cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[i])
            primes[prime_cnt++] = i;

    free(is_prime);
    primes_done = true;
}

/* Exponent of prime p in n! */
static inline idx_t exp_in_fact(idx_t n, idx_t p)
{
    idx_t e = 0;
    for (idx_t denom = p; denom <= n; denom *= p)
        e += n / denom;
    return e;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n=0, but return 0 safely */

    /* Determine (m,n) from the flattened index n.
       The rows are numbered starting at 1.
       Row r has r entries, so cumulative count after r rows is r(r+1)/2. */
    idx_t k = 0, tri = 0;
    while (tri < n) {
        ++k;
        tri += k;
    }
    idx_t row = k;                 /* row index r (>=1) */
    idx_t prev_tri = tri - row;    /* total up to previous row */
    idx_t col = n - prev_tri;      /* column index m (1 <= m <= row) */
    idx_t m = col;
    idx_t r = row;

    idx_t mn = m * r;          /* m*n   */
    idx_t denom = m + r - 1;   /* m+n-1 */

    /* Ensure prime list covers all needed primes. */
    if (!primes_done) build_primes_upto(MAX_PRIME);

    /* Compute the result as product of prime powers. */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < prime_cnt && primes[i] <= mn; ++i) {
        idx_t p = primes[i];
        idx_t e = exp_in_fact(mn, p) + exp_in_fact(m, p) + exp_in_fact(r, p)
                  - exp_in_fact(denom, p);
        for (idx_t j = 0; j < e; ++j)
            result *= (seq_t)p;
    }

    return result;
}
