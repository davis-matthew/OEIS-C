/*
 * A319316  Numbers k such that A090616(k) < A054861(k).
 * https://oeis.org/A319316
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th positive integer k such that the exponent of 3 in k!
 *            is strictly greater than the exponent of 4 in k! (=
 *            floor(v2(k!)/2))
 * Range:     u8 n <= 62, u16 n <= 2227, u32 n <= 2227, u64 n <= 2227,
 *            u128 n <= 2227, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *terms   = NULL;   /* cached list of terms          */
static size_t terms_len = 0;    /* number of terms cached       */
static size_t terms_cap = 0;    /* capacity of cache array      */
static idx_t  next_k  = 1;      /* next integer to test         */

/* exponent of prime p dividing k! */
static inline seq_t vp(idx_t k, idx_t p)
{
    seq_t sum = 0;
    idx_t t = k;
    while (t) {
        t /= p;
        sum += (seq_t)t;
    }
    return sum;
}

/* Ensure that we have at least n terms cached. */
static void ensure_terms(idx_t n)
{
    while (terms_len < (size_t)n) {
        seq_t v2 = vp(next_k, 2);
        seq_t v3 = vp(next_k, 3);
        /* condition: v3 > floor(v2/2)  (i.e. v3 > (v2 >> 1)) */
        if (v3 > (v2 >> 1)) {
            if (terms_len == terms_cap) {
                terms_cap = terms_cap ? terms_cap * 2 : 64;
#ifdef __cplusplus
                terms = reinterpret_cast<seq_t *>(
                    realloc(terms, terms_cap * sizeof(seq_t)));
#else
                terms = (seq_t *)realloc(terms,
                    terms_cap * sizeof(seq_t));
#endif
                if (!terms) abort();
            }
            terms[terms_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but safe return for n=0 */
    ensure_terms(n);
    return terms[n - 1];
}
