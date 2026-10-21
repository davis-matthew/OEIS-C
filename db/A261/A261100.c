/*
 * A261100  a(n) is the greatest m for which A002182(m) <= n; the least
 *          monotonic left inverse for highly composite numbers A002182.
 * https://oeis.org/A261100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of highly composite numbers ≤ n * * A002182
 *            lists the highly composite numbers; this function * returns the
 *            count of those ≤ n, i.e. the left‑inverse of that sequence. *
 *            The implementation builds a dynamic table of HCNs up to the
 *            largest n * seen so far, and then performs a bina...
 * Range:     u8 n <= 9804, u16 n <= 9804, u32 n <= 9804, u64 n <= 9804,
 *            u128 n <= 9804, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* static cache : list of highly composite numbers, stored as idx_t */
static idx_t *hcn_arr = NULL;        /* array of HCN values            */
static idx_t  hcn_len = 0;           /* number of elements in hcn_arr   */
static idx_t  max_computed = 0;      /* largest n for which HCN were built */

/* rebuild the list of highly composite numbers up to 'limit' */
static void rebuild_hcn(idx_t limit)
{
    if (limit <= max_computed) return;   /* nothing to do */

    /* allocate temporary arrays */
    idx_t *spf   = (idx_t*) calloc((size_t)(limit + 1), sizeof(idx_t));
    idx_t *divcnt = (idx_t*) malloc((size_t)(limit + 1) * sizeof(idx_t));
    idx_t *tmp   = (idx_t*) malloc((size_t)(limit + 1) * sizeof(idx_t));
    if (!spf || !divcnt || !tmp) abort();  /* allocation failure */

    /* compute smallest prime factor (spf) for every number ≤ limit */
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {  /* i is prime */
            spf[i] = i;
            if (i <= limit / i) {
                for (idx_t j = i * i; j <= limit; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }

    /* compute divisor counts using the spf array */
    divcnt[1] = 1;
    for (idx_t i = 2; i <= limit; ++i) {
        idx_t p = spf[i];
        idx_t m = i / p;
        idx_t exp = 1;
        while (m % p == 0) {
            ++exp;
            m /= p;
        }
        divcnt[i] = divcnt[m] * (exp + 1);
    }

    /* collect highly composite numbers */
    size_t len = 0;
    tmp[len++] = 1;            /* 1 is always a highly composite number */
    idx_t max_div = 1;

    for (idx_t i = 2; i <= limit; ++i) {
        if (divcnt[i] > max_div) {
            tmp[len++] = i;
            max_div = divcnt[i];
        }
    }

    /* replace the old list with the new one */
    free(hcn_arr);
    hcn_arr = (idx_t*) malloc(len * sizeof(idx_t));
    if (!hcn_arr) abort();

    for (size_t i = 0; i < len; ++i) hcn_arr[i] = tmp[i];

    /* free temporary arrays */
    free(tmp);
    free(divcnt);
    free(spf);

    /* update state */
    hcn_len      = (idx_t)len;
    max_computed = limit;
}

/* Public function: a(idx_t n) returns seq_t */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is undefined for n=0, safe return */

    /* rebuild cache if a larger n is requested */
    if (n > max_computed) rebuild_hcn(n);

    /* binary search to find the greatest HCN ≤ n */
    idx_t lo = 0;
    idx_t hi = hcn_len - 1;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if (hcn_arr[mid] <= n)
            lo = mid;
        else
            hi = mid - 1;
    }

    /* a(n) is the 1‑based index of that HCN */
    return (seq_t)(lo + 1);
}
