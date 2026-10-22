/*
 * A383832  a(n) = Sum_{k=0..n} (k+1) * 3^k * binomial(2*n+2,n-k).
 * https://oeis.org/A383832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (k+1) * 3^k * binomial(2*n+2, n-k) D‑finite
 *            recurrence: a(0)=1, a(1)=10, a(2)=78 For n≥3: a(n) =
 *            ((188*n-46)*a(n-1) - 192*(4*n-3)*a(n-2) + (1024*n-1536)*a(n-3))
 *            / (15*n)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */
#include <stdint.h>   /* for uint64_t */

static seq_t *rec_cache = NULL;
static size_t   rec_cap   = 0;
static idx_t    rec_last  = 0;

static void ensure_rec_cache(idx_t n)
{
    if (rec_cache == NULL) {
        size_t init = n < 3 ? 3 : n + 1;
        rec_cache = (seq_t*)malloc(init * sizeof(seq_t));
        rec_cap   = init;
        rec_cache[0] = (seq_t)1;
        rec_cache[1] = (seq_t)10;
        rec_cache[2] = (seq_t)78;
        rec_last    = 2;
    } else if ((size_t)n + 1 > rec_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(rec_cache, new_cap * sizeof(seq_t));
        rec_cache = tmp;
        rec_cap   = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Base cases handled by the cache itself */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)78;

    ensure_rec_cache(n);

    if ((size_t)n <= rec_last) return rec_cache[n];

#ifdef __cplusplus
    /* C++ (possibly arbitrary‑precision) implementation */
    for (idx_t i = rec_last + 1; i <= n; ++i) {
        seq_t a1 = rec_cache[(size_t)(i - 1)];
        seq_t a2 = rec_cache[(size_t)(i - 2)];
        seq_t a3 = rec_cache[(size_t)(i - 3)];

        /* Coefficients are within 64 bits */
        seq_t coeff1 = (seq_t)(188 * i - 46);
        seq_t coeff2 = (seq_t)(192 * (4 * i - 3));
        seq_t coeff3 = (seq_t)(1024 * i - 1536);

        seq_t num = coeff1 * a1;
        num += coeff3 * a3;
        num -= coeff2 * a2;           /* subtraction is safe: the result is non‑negative */

        seq_t den = (seq_t)(15 * i);
        rec_cache[(size_t)i] = num / den;   /* exact division */
    }
#else
    /* C implementation using 128‑bit intermediate to avoid overflow */
    for (idx_t i = rec_last + 1; i <= n; ++i) {
        seq_t a1 = rec_cache[(size_t)(i - 1)];
        seq_t a2 = rec_cache[(size_t)(i - 2)];
        seq_t a3 = rec_cache[(size_t)(i - 3)];

        unsigned __int128 n128   = (unsigned __int128)i;
        unsigned __int128 coeff1 = (unsigned __int128)(188) * n128 - 46;
        unsigned __int128 coeff2 = (unsigned __int128)192 * ((unsigned __int128)4 * n128 - 3);
        unsigned __int128 coeff3 = (unsigned __int128)1024 * n128 - 1536;

        unsigned __int128 num = coeff1 * (unsigned __int128)a1
                              + coeff3 * (unsigned __int128)a3
                              - coeff2 * (unsigned __int128)a2;

        unsigned __int128 den = (unsigned __int128)15 * n128;
        rec_cache[(size_t)i] = (seq_t)(num / den);
    }
#endif

    rec_last = n;
    return rec_cache[(size_t)n];
}
