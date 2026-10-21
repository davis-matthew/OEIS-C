/*
 * A279728  Sum of the larger parts of the Goldbach partitions (p,q) of 2n
 *          such that all primes from p to q (inclusive) appear as a part in
 *          some Goldbach partition of p+q = 2n.
 * https://oeis.org/A279728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of the larger parts q of the Goldbach partitions
 *            (p,q) of 2n such that every prime r with p ≤ r ≤ q (inclusive)
 *            appears in some Goldbach partition of 2n. For n < 3 the value is
 *            0
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Helper: build all primes up to 'limit' (inclusive) and an array indicating primality. */
static void build_primes(idx_t limit, idx_t **primes_out, size_t *cnt_out,
                         unsigned char **prime_mark_out)
{
    /* Allocate primality bitmap. */
    unsigned char *prime_mark = (unsigned char *)malloc((size_t)(limit + 1));
    if (!prime_mark) exit(1);
    memset(prime_mark, 1, (size_t)(limit + 1));
    if (limit >= 0) prime_mark[0] = 0;
    if (limit >= 1) prime_mark[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= limit; ++i)
        if (prime_mark[i])
            for (idx_t j = i * i; j <= limit; j += i)
                prime_mark[j] = 0;

    /* Count primes. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_mark[i]) ++cnt;

    /* Allocate array of primes. */
    idx_t *primes = (idx_t *)malloc(cnt * sizeof(idx_t));
    if (!primes) exit(1);
    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (prime_mark[i]) primes[pos++] = i;

    *primes_out     = primes;
    *cnt_out        = cnt;
    *prime_mark_out = prime_mark;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* no Goldbach partition for n < 3 */

    static idx_t global_max_limit = 0;
    static idx_t *primes          = NULL;
    static size_t primes_cnt      = 0;
    static unsigned char *is_prime = NULL;
    static unsigned char *present   = NULL;
    static idx_t present_cap = 0;

    idx_t needed_limit = 2 * n;
    if (needed_limit < n)          /* overflow guard – treat as huge */
        needed_limit = SIZE_MAX;

    /* Rebuild prime tables if needed. */
    if (needed_limit > global_max_limit) {
        if (is_prime) free(is_prime);
        if (primes)  free(primes);
        build_primes(needed_limit, &primes, &primes_cnt, &is_prime);
        global_max_limit = needed_limit;
    }

    /* Ensure present array has enough space. */
    idx_t needed_present = needed_limit + 1;
    if (present_cap < needed_present) {
        unsigned char *new_present = (unsigned char *)
            realloc(present, (size_t)needed_present);
        if (!new_present) exit(1);
        present      = new_present;
        present_cap  = needed_present;
    }
    memset(present, 0, (size_t)needed_present);   /* clear for current n */

    /* First pass: mark all primes that appear in any Goldbach partition of 2n. */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = primes[pi];
        if (p > n) break;
        idx_t q = 2 * n - p;
        if (q < p) continue;
        if (q > needed_limit) continue;        /* safety */
        if (is_prime[q]) {
            present[p] = 1;
            present[q] = 1;
        }
    }

    /* Second pass: sum the larger parts of qualifying partitions. */
    seq_t sum = 0;
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        idx_t p = primes[pi];
        if (p > n) break;
        idx_t q = 2 * n - p;
        if (q < p) continue;
        if (q > needed_limit) continue;
        if (!is_prime[q]) continue;

        bool ok = true;
        /* check all primes r with p ≤ r ≤ q */
        for (size_t pj = pi; pj < primes_cnt; ++pj) {
            idx_t r = primes[pj];
            if (r > q) break;
            if (!present[r]) { ok = false; break; }
        }
        if (ok)
            sum += (seq_t)q;
    }
    return sum;
}
