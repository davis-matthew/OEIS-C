/*
 * A289851  Primes of the form k!2 - 8, where k!2 is the double factorial
 *          number (A006882).
 * https://oeis.org/A289851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Helpers:   ensure_capacity, is_prime_big, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289851: Primes of the form k!! - 8, where k!! is the
   double factorial of k (A006882).  The function returns the
   n-th such prime (n >= 1). */
#include <stdlib.h>
#include "../utils/ensure_capacity.h"
#include "../utils/mul_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static idx_t  primes_cnt = 0;
    static seq_t  double_fact = 0;
    static idx_t  last_k = 0;

    /* initialise on first call */
    if (primes == NULL) {
        primes_cap = 16; /* arbitrary initial capacity */
        primes = (seq_t *)malloc(primes_cap * sizeof(seq_t));
        double_fact = 1;  /* 1!! = 1 */
        last_k = 1;       /* k will advance from 1 to 3,5,7,... */
    }

    /* generate further primes until we have at least n of them */
    while (primes_cnt < n) {
        idx_t k = last_k + 2;                      /* next odd k */
        double_fact = mul_small(double_fact, (unsigned int)k); /* k!! = previous * k */
        seq_t val = double_fact - 8;              /* candidate prime */

        if (is_prime_big(val)) {
            /* grow array if necessary */
            ensure_capacity(primes_cnt + 1, &primes_cap,
                            (void **)&primes, sizeof(seq_t));
            primes[primes_cnt++] = val;
        }
        last_k = k;
    }

    return primes[n-1];
}
