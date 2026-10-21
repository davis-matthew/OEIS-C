/*
 * A264719  Numbers that are greater than the average of their closest
 *          flanking primes.
 * https://oeis.org/A264719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth integer m>1 such that m is greater than the
 *            average of its closest flanking primes. If p_prev < m < p_next
 *            are the two consecutive primes nearest to m, the condition is
 *            2*m > p_prev + p_next
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* allocation helpers that work in both C and C++ */
#if defined(__cplusplus)
    #define SEQ_ALLOC(n)        new seq_t[n]
    #define SEQ_FREE(p)         delete[] (p)
    #define SEQ_REALLOC(p, cap, newcap) do {    \
        seq_t* newp = new seq_t[newcap];        \
        for (size_t i = 0; i < (cap); ++i) newp[i] = (p)[i]; \
        delete[] (p);                            \
        (p) = newp;                               \
        (cap) = (newcap);                         \
    } while (0)
#else
    #define SEQ_ALLOC(n)        ((seq_t*)malloc(sizeof(seq_t)*(n)))
    #define SEQ_FREE(p)         free(p)
    #define SEQ_REALLOC(p, cap, newcap) do {    \
        (p) = realloc((p), sizeof(seq_t)*(newcap)); \
        (cap) = (newcap);                                 \
    } while (0)
#endif

/* simple deterministic primality test (sufficient for the intended range) */
static bool is_prime_u64(idx_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (idx_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

seq_t a(idx_t n)
{
    static idx_t collected  = 0;
    static seq_t *cache     = NULL;
    static size_t cache_cap = 0;
    static idx_t cur_num    = 4;     /* start scanning from the first composite > 3 */
    static idx_t prev_prime = 3;     /* largest prime < cur_num */
    static idx_t next_prime = 5;     /* smallest prime > cur_num */

    /* allocate initial cache if needed */
    if (cache == NULL) {
        cache_cap = 8;
        cache = SEQ_ALLOC(cache_cap);
    }

    /* generate terms until we have at least n of them */
    while (collected < n) {
        /* make sure next_prime is strictly greater than cur_num */
        if (next_prime == cur_num) {
            idx_t cand = cur_num + 1;
            while (!is_prime_u64(cand)) cand++;
            next_prime = cand;
        }

        /* check the OEIS condition */
        if ((seq_t)2 * (seq_t)cur_num > (seq_t)prev_prime + (seq_t)next_prime) {
            if (collected >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                SEQ_REALLOC(cache, cache_cap, new_cap);
            }
            cache[collected++] = (seq_t)cur_num;
        }

        /* update prime markers for the next loop */
        if (is_prime_u64(cur_num))
            prev_prime = cur_num;

        cur_num++;
    }

    return cache[n - 1];
}
