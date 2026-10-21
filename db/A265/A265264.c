/*
 * A265264  Terms satisfy: a(2*n) = a(n)*b(n) and a(2*n+1) = a(n+1)*b(n) for
 *          n>=0 with a(0)=1, where A(x)^2 = Sum_{n>=0} b(n)*x^n and g.f. A(x)
 *          = Sum_{n>=0} a(n)*x^n.
 * https://oeis.org/A265264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 50, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1; for n≥2 let m=floor(n/2);
   if n even  a(n)=a(m)*b(m),
   if n odd   a(n)=a(m+1)*b(m);
   where b(m)=Σ_{k=0}^{m} a(k)*a(m−k). */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* static cache for a(n) and b(n) */
    static seq_t *a_cache   = NULL;
    static seq_t *b_cache   = NULL;
    static idx_t a_cap      = 0;
    static idx_t b_cap      = 0;

    /* allocate or grow caches when needed */
    if (n >= a_cap) {
        idx_t new_a_cap = (a_cap == 0) ? 16 : a_cap;
        while (new_a_cap <= n) new_a_cap <<= 1;

        a_cache = (seq_t *)realloc(a_cache, (new_a_cap + 1) * sizeof(seq_t));
        b_cache = (seq_t *)realloc(b_cache, (new_a_cap / 2 + 2) * sizeof(seq_t));

        /* initialize new entries to zero */
        memset(a_cache + a_cap, 0, (new_a_cap - a_cap + 1) * sizeof(seq_t));
        memset(b_cache + b_cap, 0, (new_a_cap / 2 + 2 - b_cap) * sizeof(seq_t));

        a_cap = new_a_cap;
        b_cap = new_a_cap / 2 + 2;
    }

    /* initialise base terms if not yet done */
    if (a_cache[0] == 0) {
        a_cache[0] = 1;
        a_cache[1] = 1;
        b_cache[0] = 1;
    }

    /* compute terms up to n in ascending order */
    for (idx_t i = 2; i <= n; ++i) {
        if (a_cache[i] != 0) continue;          /* already computed */

        if ((i & 1) == 0) {                     /* even index: a(2*m) */
            idx_t m = i >> 1;
            if (b_cache[m] == 0) {              /* compute b(m) if needed */
                seq_t sum = 0;
                for (idx_t k = 0; k <= m; ++k)
                    sum += a_cache[k] * a_cache[m - k];
                b_cache[m] = sum;
            }
            a_cache[i] = a_cache[m] * b_cache[m];
        } else {                                /* odd index: a(2*m+1) */
            idx_t m = (i - 1) >> 1;
            if (b_cache[m] == 0) {              /* compute b(m) if needed */
                seq_t sum = 0;
                for (idx_t k = 0; k <= m; ++k)
                    sum += a_cache[k] * a_cache[m - k];
                b_cache[m] = sum;
            }
            a_cache[i] = a_cache[m + 1] * b_cache[m];
        }
    }

    return a_cache[n];
}
