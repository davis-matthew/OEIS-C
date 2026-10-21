/*
 * A278934  a(n) = Sum_{k=0..n} (-1)^(n-k)*binomial(n,k)*binomial(2*k,k)^2.
 * https://oeis.org/A278934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} (-1)^{n-k} * binomial(n,k) * binomial(2*k,k)^2
 *            Recurrence: n^2*a(n) =
 *            (13*n^2-13*n+3)*a(n-1)+29*(n-1)^2*a(n-2)+15*(n-2)*(n-1)*a(n-3)
 *            Initial values: a(0)=1, a(1)=3, a(2)=29
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;      /* computed values */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t  cache_next = 0;    /* first index not yet computed */

seq_t a(idx_t n)
{
    /* Compute all values up to n, storing them in the cache */
    while (cache_next <= n) {
        seq_t val;

        if (cache_next < 3) {            /* initial values */
            switch (cache_next) {
                case 0: val = (seq_t)1; break;
                case 1: val = (seq_t)3; break;
                case 2: val = (seq_t)29; break;
            }
        } else {                           /* recurrence */
            if (sizeof(seq_t) <= 8) {
                /* Use 128‑bit intermediates for built‑in small integer types */
                unsigned __int128 i      = (unsigned __int128)cache_next;
                unsigned __int128 i2     = i * i;
                unsigned __int128 coeff1 = 13 * i2 - 13 * i + 3;
                unsigned __int128 coeff2 = 29 * (i - 1) * (i - 1);
                unsigned __int128 coeff3 = 15 * (i - 2) * (i - 1);

                unsigned __int128 term1 = coeff1 * (unsigned __int128)cache[cache_next-1];
                unsigned __int128 term2 = coeff2 * (unsigned __int128)cache[cache_next-2];
                unsigned __int128 term3 = coeff3 * (unsigned __int128)cache[cache_next-3];
                unsigned __int128 num   = term1 + term2 + term3;

                val = (seq_t)(num / i2);
            } else {
                /* Use seq_t arithmetic for arbitrary‑precision or larger built‑in types */
                seq_t si      = (seq_t)cache_next;
                seq_t i2      = si * si;
                seq_t coeff1  = 13 * i2 - 13 * si + (seq_t)3;
                seq_t coeff2  = 29 * ((si - (seq_t)1) * (si - (seq_t)1));
                seq_t coeff3  = 15 * ((si - (seq_t)2) * (si - (seq_t)1));

                seq_t term1 = coeff1 * cache[cache_next-1];
                seq_t term2 = coeff2 * cache[cache_next-2];
                seq_t term3 = coeff3 * cache[cache_next-3];
                seq_t num   = term1 + term2 + term3;

                val = num / i2;
            }
        }

        /* Grow storage if necessary */
        if ((size_t)cache_next >= cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap <= (size_t)cache_next) new_cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();    /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_next++] = val;
    }

    return cache[n];
}
