/*
 * A368040  The powerful part of the nonsquarefree numbers.
 * https://oeis.org/A368040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A057521(A013929(n)) – the powerful part of the n‑th
 *            nonsquarefree integer
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static idx_t last_checked = 1;   /* last integer examined (1‑based)   */
static idx_t count_nofsq = 0;    /* amount of nonsquarefree numbers
                                   computed so far (1‑based)        */
static seq_t *cache = NULL;      /* cache of a(1)..a(count_nofsq)    */
static size_t cache_cap = 0;     /* allocated size of `cache`         */

/* Ensure `cache` can hold at least `needed` elements.  */
static void ensure_cache_cap(idx_t needed)
{
    if ((size_t)needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while ((size_t)needed > new_cap) new_cap *= 2;
    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;
    cache_cap = new_cap;
}

/* Return the powerful part of the n‑th nonsquarefree number.  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 – no 0‑th term */
    if (count_nofsq >= n) return cache[n-1];   /* already computed */

    while (count_nofsq < n) {
        ++last_checked;                       /* examine next integer    */
        uint64_t x = last_checked;
        seq_t part = 1;
        uint64_t tmp = x;

        /* Factorize `tmp`, building the powerful part from
           exponents >= 2.  */
        for (uint64_t p = 2; p * p <= tmp; ++p) {
            if (tmp % p == 0) {
                int e = 0;
                do {
                    tmp /= p;
                    ++e;
                } while (tmp % p == 0);
                if (e >= 2) {
                    seq_t pow = 1;
                    for (int i = 0; i < e; ++i) pow *= (seq_t)p;
                    part *= pow;
                }
            }
        }
        /* If a prime factor > sqrt(tmp) remains, its exponent is 1
           – ignore it for the powerful part.  */

        if (part != 1) {  /* `x` is nonsquarefree                      */
            ++count_nofsq;
            ensure_cache_cap(count_nofsq);
            cache[count_nofsq-1] = part;
        }
    }
    return cache[n-1];
}
