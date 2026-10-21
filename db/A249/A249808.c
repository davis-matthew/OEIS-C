/*
 * A249808  Triangular table read by rows, a lower right triangular region of
 *          square array A(n,k) is the number of times prime p_k has occurred
 *          as the smallest prime factor of numbers 1..n.
 * https://oeis.org/A249808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9659, u16 n <= 9659, u32 n <= 9659, u64 n <= 9659,
 *            u128 n <= 9659, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Sequence A249808 – triangular table read by rows.  */
/* a(n) = A(row(k),col(k)) where row(k) is the smallest r with
   r(r+1)/2 >= k.  The table entry A(r,i) is the number of integers
   1..r whose smallest prime factor is the i‑th prime.           */

/* The built‑in types seq_t and idx_t are supplied by the build
   configuration.  They may be any unsigned integer type, big integer
   class, or arbitrary‑precision class.  The code below works for
   all of them.                                                     */

static idx_t  max_tri_cached = 0;      /* largest n for which cache is valid   */
static seq_t *cache          = NULL;    /* 1‑indexed array of a(n) values       */

/* Compute r such that r(r+1)/2 >= n, n>=1.  */
static idx_t
tri_row(idx_t n)
{
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) ++r;
    return r;
}

/* Build or extend the cache so that it contains all terms up to n. */
static void
ensure_cache(idx_t n)
{
    if (n <= max_tri_cached) return;   /* already built                         */

    /* Find the row that covers n */
    idx_t row = tri_row(n);
    idx_t new_tri = row * (row + 1) / 2;      /* length required, 1‑indexed           */

    /* Reallocate the cache array if necessary */
    seq_t *tmp = (seq_t *)realloc(cache, (new_tri + 1) * sizeof(seq_t));
    if (!tmp) return;                         /* allocation failure – keep old cache  */
    cache = tmp;
    /* No need to zero the new part – values will be overwritten */

    /* Sieve of Eratosthenes for smallest prime factor up to 'row' */
    uint32_t *spf = (uint32_t *)calloc((size_t)row + 1, sizeof(uint32_t));
    uint32_t *prime_index = (uint32_t *)calloc((size_t)row + 1, sizeof(uint32_t));
    if (!spf || !prime_index) return;         /* allocation failure – keep old cache  */

    /* Linear sieve: small enough for row <= ~150, but this implementation
       works for any row that fits in memory. */
    size_t prime_cnt = 0;
    uint32_t *primes = (uint32_t *)malloc((size_t)row * sizeof(uint32_t));
    if (!primes) { free(spf); free(prime_index); return; }

    for (idx_t i = 2; i <= row; ++i) {
        if (spf[i] == 0) {                    /* i is prime */
            spf[i] = (uint32_t)i;
            prime_index[i] = (uint32_t)prime_cnt;
            primes[prime_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < prime_cnt; ++j) {
            uint32_t p = primes[j];
            uint64_t mul = (uint64_t)i * p;
            if (mul > row) break;
            if (spf[mul] == 0) spf[mul] = p;
            if (i % p == 0) break;
        }
    }

    /* Cumulative counts of smallest prime factors */
    seq_t *cumul = (seq_t *)calloc(prime_cnt, sizeof(seq_t));
    if (!cumul) { free(spf); free(prime_index); free(primes); return; }

    /* Fill the cache */
    size_t pos = 1;                           /* cache is 1‑indexed */
    for (idx_t r = 1; r <= row; ++r) {
        if (r >= 2) {
            uint32_t p = spf[r];
            uint32_t idxp = prime_index[p];
            if (idxp < prime_cnt)              /* safety check */
                cumul[idxp] += 1;
        }
        for (idx_t k = 1; k <= r; ++k) {
            seq_t val = (k <= prime_cnt) ? cumul[k - 1] : 0;
            cache[pos++] = val;
        }
    }

    /* Clean up */
    free(spf);
    free(prime_index);
    free(primes);
    free(cumul);

    max_tri_cached = new_tri;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    return cache[n];
}
