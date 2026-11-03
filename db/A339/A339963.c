/*
 * A339963  Numbers k such that gcd(k+1, sigma(k)) is 2.
 * https://oeis.org/A339963
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 4451, u32 n <= 16023, u64 n <= 16023,
 *            u128 n <= 16023, big any n
 * Validated: standard, pass, 80 terms through n = 9761 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the k-th integer k for which gcd(k+1, sigma(k)) = 2.
   No closed form is known; this routine enumerates k incrementally
   and caches the qualifying values. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

static void grow_terms(seq_t **arr, size_t *cap, size_t needed)
{
    if (*cap >= needed) return;
    size_t new_cap = *cap ? *cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, *arr, new_cap);
    if (!tmp) abort();
    *arr = tmp;
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1 */

    /* Static state: cache of found terms and last k examined. */
    static seq_t *terms = NULL;          /* cached k values */
    static size_t terms_cap = 0;          /* allocated capacity of terms */
    static size_t terms_len = 0;          /* number of stored terms */
    static idx_t last_k = 0;              /* last k that was tested */

    /* Generate enough terms to satisfy the request. */
    while (terms_len < (size_t)n)
    {
        last_k++;                                   /* next candidate k */

        seq_t sigma = sigma_of(last_k);             /* σ(k) */
        seq_t g = gcd((seq_t)(last_k + 1), sigma);  /* gcd(k+1, σ(k)) */

        if (g == 2)                                 /* qualifying k */
        {
            grow_terms(&terms, &terms_cap, terms_len + 1);
            terms[terms_len++] = (seq_t)last_k;
        }
    }

    return terms[n - 1];
}
