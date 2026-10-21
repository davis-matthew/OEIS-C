/*
 * A283567  Number of nX3 0..1 arrays with no 1 equal to more than one of its
 *          horizontal, diagonal and antidiagonal neighbors, with the
 *          exception of exactly one element.
 * https://oeis.org/A283567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(n) = 10*a(n-1)-27*a(n-2)-4*a(n-3)+89*a(n-4)-106*a(n-5)-85*a(n-6)
          +228*a(n-7)-60*a(n-8)-192*a(n-9)+144*a(n-10)+64*a(n-11)-64*a(n-12);
   Initial terms: a(1)=1, a(2)=26, a(3)=169, a(4)=1108, a(5)=6453,
                  a(6)=36038, a(7)=194173, a(8)=1021432,
                  a(9)=5275885, a(10)=26869458, a(11)=135310457,
                  a(12)=675163708.
*/

#include <stdlib.h>

static const int coeffs[12] = {
    10, -27, -4, 89, -106, -85, 228, -60, -192, 144, 64, -64
};

static const seq_t init_vals[13] = {
    0ULL, /* unused index 0 */
    1ULL, 26ULL, 169ULL, 1108ULL, 6453ULL,
    36038ULL, 194173ULL, 1021432ULL, 5275885ULL, 26869458ULL,
    135310457ULL, 675163708ULL
};

static idx_t cache_size = 0;
static idx_t cache_cap   = 0;
static seq_t *cache      = NULL;

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Allocate or grow cache if necessary */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) abort();   /* Out of memory */
        cache = tmp;
        cache_cap = (idx_t)new_cap;
    }

    /* Compute missing terms incrementally */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        if (i <= 12) {
            cache[i] = init_vals[i];
        } else {
            seq_t pos = 0, neg = 0;
            for (int k = 0; k < 12; ++k) {
                seq_t term  = cache[i - 1 - k];
                int   coeff = coeffs[k];
                seq_t prod  = (seq_t)((coeff >= 0) ? coeff : -coeff) * term;
                if (coeff >= 0)
                    pos += prod;
                else
                    neg += prod;
            }
            cache[i] = pos - neg;  /* pos > neg because result is positive */
        }
    }

    cache_size = n;
    return cache[n];
}
