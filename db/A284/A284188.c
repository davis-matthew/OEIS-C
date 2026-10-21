/*
 * A284188  a(1)=2; thereafter a(n+1) = a(n)+i if a(n) is a prime and
 *          a(1),...,a(n) contains i primes, or a(n+1) = a(n)-i if a(n) is
 *          composite and a(1),...,a(n) contains i primes.
 * https://oeis.org/A284188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; for n≥1 let p_n be the number of primes among
 *            a(1),...,a(n). Then a(n+1)=a(n)+p_n if a(n) is prime,
 *            a(n+1)=a(n)-p_n if a(n) is composite
 * Range:     u8 n <= 60, u16 n <= 1859, u32 n <= 2139, u64 n <= 9274,
 *            u128 n <= 9274, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *vals   = NULL;   /* a(1), a(2), ... */
    static seq_t *primes = NULL;   /* number of primes up to that term */
    static size_t   cap   = 0;     /* allocated capacity of the arrays    */
    static idx_t    len   = 0;     /* number of terms already computed    */

    if (n == 0) return (seq_t)0;    /* offset is 1; n=0 is outside the sequence */

    /* Initialise the cache on the first call. */
    if (len == 0) {
        size_t init = 16;
        vals   = (seq_t*)malloc(init * sizeof(seq_t));
        primes = (seq_t*)malloc(init * sizeof(seq_t));
        vals[0]   = (seq_t)2;  /* a(1) = 2 */
        primes[0] = (seq_t)1;  /* one prime seen so far          */
        len   = 1;
        cap   = init;
    }

    /* Grow the arrays if the requested index is beyond current capacity. */
    size_t need = (size_t)n;
    if (cap < need) {
        size_t new_cap = cap ? cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        vals   = (seq_t*)realloc(vals,   new_cap * sizeof(seq_t));
        primes = (seq_t*)realloc(primes, new_cap * sizeof(seq_t));
        cap = new_cap;
    }

    /* Compute terms up to the requested index. */
    for (idx_t i = len; i < n; ++i) {
        seq_t last   = vals[i-1];
        bool  lastp  = is_prime_big(last);
        seq_t pcount = primes[i-1];                     /* primes among a(1)…a(i) */

        seq_t next = lastp ? last + pcount : last - pcount;
        vals[i]   = next;

        bool nextp = is_prime_big(next);
        primes[i] = primes[i-1] + (nextp ? (seq_t)1 : (seq_t)0);
    }

    len = n;
    return vals[n-1];
}
