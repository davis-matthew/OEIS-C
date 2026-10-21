/*
 * A279737  Number of nX4 0..1 arrays with no element equal to a strict
 *          majority of its horizontal and antidiagonal neighbors, with the
 *          exception of exactly one element, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A279737
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A279737
   a(n) = 20*a(n-1) - 166*a(n-2) + 782*a(n-3) - 2465*a(n-4) + 5714*a(n-5)
          - 10213*a(n-6) + 14398*a(n-7) - 16186*a(n-8) + 14502*a(n-9)
          - 10252*a(n-10) + 5622*a(n-11) - 2332*a(n-12) + 704*a(n-13)
          - 145*a(n-14) + 18*a(n-15) - a(n-16)   for n > 17
   initial values:
     a(1) = 2,  a(2) = 14,  a(3) = 106,  a(4) = 736,  a(5) = 4940,
     a(6) = 32430, a(7) = 209558, a(8) = 1337624, a(9) = 8453760,
     a(10) = 52990574, a(11) = 329875212, a(12) = 2041484910,
     a(13) = 12570123264, a(14) = 77057213940, a(15) = 470543267950,
     a(16) = 2863457284456, a(17) = 17371926764454
*/
#include <stdlib.h>
#include <stdint.h>

/* signed coefficients of the recurrence, from a(n-1) to a(n-16) */
static const int coeffs[16] = {
    20, -166, 782, -2465, 5714, -10213, 14398,
    -16186, 14502, -10252, 5622, -2332, 704, -145, 18, -1
};

/* initial 17 terms (index 0 unused) */
static const seq_t init[18] = {
    0ULL,                                 /* a(0) – unused */
    2ULL, 14ULL, 106ULL, 736ULL, 4940ULL,
    32430ULL, 209558ULL, 1337624ULL,
    8453760ULL, 52990574ULL, 329875212ULL,
    2041484910ULL, 12570123264ULL,
    77057213940ULL, 470543267950ULL,
    2863457284456ULL, 17371926764454ULL
};

static seq_t *cache = NULL;     /* cache[0..cached_len] */
static size_t cached_len = 0;   /* number of terms stored (>=0) */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0 */

    /* initialise cache with the first 17 terms on first call */
    if (cache == NULL) {
        /* allocate space for indices 0..17 */
        cache = (seq_t *)malloc((17 + 1) * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure */
        for (size_t i = 0; i <= 17; ++i) cache[i] = init[i];
        cached_len = 17;
    }

    /* if requested term is already cached, return it */
    if ((size_t)n <= cached_len) return cache[(size_t)n];

    /* extend cache to contain up to index n */
    size_t old_len = cached_len;
    size_t new_len = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_len + 1) * sizeof(seq_t));
    if (new_cache == NULL) return 0; /* reallocation failure */
    cache = new_cache;
    cached_len = new_len;

    /* compute missing terms using the recurrence */
    for (size_t i = old_len + 1; i <= new_len; ++i) {
        seq_t sum = 0;
        for (int k = 0; k < 16; ++k) {
            size_t idx = i - (size_t)(k + 1);   /* a(i-(k+1)) */
            seq_t term = cache[idx];
            int c = coeffs[k];
            seq_t prod = term * (seq_t)c;        /* uses seq_t × int */
            sum += prod;
        }
        cache[i] = sum;
    }

    return cache[(size_t)n];
}
