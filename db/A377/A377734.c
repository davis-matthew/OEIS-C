/*
 * A377734  Number of integers less than n that have the same smallest prime
 *          factor as n.
 * https://oeis.org/A377734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ j : 1 ≤ j < n and lpf(j) = lpf(n) }|, where lpf is the
 *            smallest prime factor. The function computes the value by
 *            sieving the smallest prime factor for all integers up to the
 *            requested bound, and keeping a running count of how many numbers
 *            have each prime as their smallest prime factor....
 * Range:     u8 n <= 513, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* static state for on‑demand sieving */
static idx_t            max_sieved = 0;     /* largest n that has been sieved */
static uint64_t       *spf_arr   = NULL;     /* smallest prime factor array  */
static seq_t          *cnt_arr   = NULL;     /* counts of numbers with a given lpf */

/* Recompute the sieve up to n (inclusive).  The function frees any previously
   allocated arrays and allocates new ones large enough for n+1 entries. */
static void sieve_up_to(idx_t n)
{
    if (n <= max_sieved)
        return;                     /* already sufficient */

    /* allocate fresh arrays */
    free(spf_arr);
    free(cnt_arr);
    spf_arr = (uint64_t *)malloc((size_t)(n + 1) * sizeof(uint64_t));
    cnt_arr = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!spf_arr || !cnt_arr)
        return;                     /* allocation failure – unlikely here */

    /* initialisation */
    for (idx_t i = 0; i <= n; ++i)
        spf_arr[i] = i;             /* spf[i] = i initially */

    /* classic sieve to compute smallest prime factors */
    for (idx_t i = 2; (idx_t)(i * i) <= n; ++i) {
        if (spf_arr[i] == i) {      /* i is prime */
            for (idx_t j = i * i; j <= n; j += i) {
                if (spf_arr[j] == j)
                    spf_arr[j] = i;
            }
        }
    }

    /* zero the counter array */
    for (idx_t i = 0; i <= n; ++i)
        cnt_arr[i] = 0;

    /* count how many times each prime appears as an lpf */
    for (idx_t i = 2; i <= n; ++i) {
        uint64_t p = spf_arr[i];      /* p is prime */
        cnt_arr[p]++;                 /* cnt_arr uses seq_t automatically */
    }

    max_sieved = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                     /* not defined for n < 1, return 0 */

    if (n == 1)
        return 0;                     /* a(1) = 0 by definition */

    /* ensure we have sieved up to n */
    sieve_up_to(n);

    /* retrieve smallest prime factor of n */
    uint64_t p = spf_arr[n];

    /* count how many numbers <= n have this lpf, subtract 1 to exclude n itself */
    seq_t count = cnt_arr[p];
    return count - 1;
}
