/*
 * A395300  Sum of numbers k < n such that rad(k) | n but k does not divide n,
 *          where rad = A007947.
 * https://oeis.org/A395300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = sum of k < n such that rad(k) | n but k ∤ n where rad(k)
 *            is the product of distinct prime factors of k (A007947). a(1) =
 *            0
 * Range:     u8 n <= 59, u16 n <= 1469, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Cached smallest prime factor table for numbers up to spf_cap-1 */
static idx_t *spf = NULL;      /* smallest prime factor of i */
static size_t spf_cap = 0;     /* array length (maximum value + 1) */

/* Extend the SPF table to cover all numbers up to limit.
   The table is built once and extended only when a larger limit is needed. */
static void ensure_spf(idx_t limit)
{
    if (spf_cap > (size_t)limit + 1) return;   /* already sufficient */

    /* free any previous table */
    if (spf != NULL) free(spf);

    /* allocate new table with indices 0..limit inclusive */
    spf = (idx_t*)malloc(sizeof(idx_t) * ((size_t)limit + 1));
    if (spf == NULL) exit(1);

    /* initialise entries */
    for (idx_t i = 0; i <= limit; ++i) spf[i] = 0;
    spf[0] = 0;
    if (limit >= 1) spf[1] = 1;

    /* sieve to fill smallest prime factors */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                 /* i is prime */
            spf[i] = i;
            if (i > limit / i) continue;   /* avoid overflow in i*i */
            for (idx_t j = i * i; j <= limit; j += i)
                if (spf[j] == 0) spf[j] = i;
        }
    }
    spf_cap = (size_t)limit + 1;
}

/* Compute the sequence term a(n).  An SPF table up to n-1 is created (or re‑used). */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    /* prepare SPF table for values < n */
    ensure_spf(n - 1);

    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k) {
        /* compute rad(k) using the SPF table */
        idx_t rad = 1;
        idx_t temp = k;
        while (temp > 1) {
            idx_t p = spf[temp];
            rad *= p;
            while (temp % p == 0) temp /= p;
        }

        if ((n % rad == 0) && (n % k != 0))
            sum += (seq_t)k;
    }
    return sum;
}
