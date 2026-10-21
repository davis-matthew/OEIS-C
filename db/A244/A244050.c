/*
 * A244050  Partial sums of A243980.
 * https://oeis.org/A244050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4; a(n)=a(n-1)+4*sum_{k=1..n} sigma(k) for n≥2
 * Range:     u8 n <= 5, u16 n <= 38, u32 n <= 1575, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sigma_value.h"

/* Cached partial sums of sigma(n) and of a(n). */
static seq_t *sigma_sums = NULL;   /* sigma_sums[i] = sum_{k=1..i} sigma(k) */
static seq_t *a_vals    = NULL;   /* a_vals[i] = a(i) */
static idx_t  current_max = 0;
static size_t array_cap   = 0;

/* Ensure that data up to index n is computed and cached. */
static void ensure_capacity(idx_t n)
{
    if (current_max >= n) return;

    size_t needed = (size_t)(n + 1);
    if (array_cap < needed) {
        size_t newcap = array_cap ? array_cap * 2 : 8;
        while (newcap < needed) newcap <<= 1;
        sigma_sums = (seq_t*)realloc(sigma_sums, newcap * sizeof(seq_t));
        a_vals    = (seq_t*)realloc(a_vals,    newcap * sizeof(seq_t));
        array_cap = newcap;
    }

    if (current_max == 0) {          /* initialise base case */
        sigma_sums[0] = a_vals[0] = (seq_t)0;
    }

    for (idx_t i = current_max + 1; i <= n; ++i) {
        seq_t sigma_i = sigma_value(i);
        sigma_sums[i] = sigma_sums[i - 1] + sigma_i;

        seq_t add = sigma_sums[i] * (seq_t)4;
        a_vals[i] = a_vals[i - 1] + add;
    }

    current_max = n;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety, sequence offset is 1 */
    ensure_capacity(n);
    return a_vals[n];
}
