/*
 * A290725  Numbers with 3k digits for some k such that the first k digits
 *          minus the middle k digits equals the last k digits.
 * https://oeis.org/A290725
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 54, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: generates all 3k‑digit numbers of the form
   X*10^(2k)+Y*10^k+(X-Y) where 10^(k-1)≤X≤10^k-1, 0≤Y≤X.
   These numbers satisfy first k digits – middle k digits = last k digits,
   and are listed in increasing order.  This routine returns the n‑th
   term (1‑based). */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;          /* buffer of generated terms        */
static size_t cache_len = 0;         /* number of terms in cache         */
static size_t cache_cap = 0;         /* allocated capacity of cache      */
static idx_t last_k = 0;             /* largest k for which terms are generated */

/* Ensure the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        ++last_k;                     /* generate terms for the next k */

        /* compute 10^k, 10^(k-1), and 10^(2k) as seq_t */
        seq_t pow10k   = 1;
        for (idx_t i = 0; i < last_k; ++i)
            pow10k *= 10;              /* 10^k */
        seq_t pow10k1  = pow10k / 10;  /* 10^(k-1) */
        seq_t pow10_2k = pow10k * pow10k; /* 10^(2k) */

        /* loop over the first k digits (X) and middle k digits (Y)   */
        idx_t X_start = (idx_t)pow10k1;     /* 10^(k-1)  */
        idx_t X_end   = (idx_t)pow10k - 1;  /* 10^k - 1   */

        for (idx_t X = X_start; X <= X_end; ++X) {
            seq_t X_seq = (seq_t)X;           /* cast once for speed */

            for (idx_t Y = 0; Y <= X; ++Y) {
                seq_t Y_seq = (seq_t)Y;
                seq_t N = X_seq * pow10_2k
                          + Y_seq * pow10k
                          + (X_seq - Y_seq);

                /* grow the cache if necessary */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    /* insure we still have room after the growth */
                    while (new_cap <= cache_len) new_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = N;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but safe return */
    ensure_cache(n);               /* generate terms up to n */
    return cache[n - 1];           /* sequence is 1‑based */
}
