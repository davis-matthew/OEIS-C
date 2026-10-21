/*
 * A271628  Primes that are the sum of the digits of the numbers from 1 to n,
 *          for some n.
 * https://oeis.org/A271628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = the nth prime that equals the cumulative sum of the *
 *            digit sums of numbers from 1 up to some m. That is, * a(n) is
 *            the nth value of `sum_{k=1}^{m} digit_sum(k)` that is prime. *
 *            (OEIS A271628)
 * Range:     u8 n <= 4, u16 n <= 155, u32 n <= 44, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *prime_list = NULL;   /* array of primes found so far */
    static size_t prime_cap  = 0;      /* current vector capacity    */
    static size_t prime_len  = 0;      /* number of primes stored    */
    static idx_t   current_m   = 0;    /* last m processed           */
    static seq_t  current_sum = 0;    /* cumulative sum up to m     */

    if (n == 0) return 0;  /* undefined index – return 0 */

    /* initialise the prime array on the first call */
    if (prime_list == NULL) {
        prime_cap  = 16;
        prime_list = (seq_t *)malloc(prime_cap * sizeof(seq_t));
        if (!prime_list) return 0;  /* allocation failure – unlikely */
    }

    /* generate primes until we have at least n of them */
    while (prime_len < (size_t)n) {
        /* move to the next integer m */
        ++current_m;

        /* add its digit sum to the running total */
        current_sum += digit_sum((seq_t)current_m);

        /* if the new total is prime, store it */
        if (is_prime_big(current_sum)) {
            if (prime_len == prime_cap) {   /* grow the array */
                prime_cap *= 2;
                seq_t *tmp = (seq_t *)realloc(prime_list, prime_cap * sizeof(seq_t));
                if (!tmp) return 0;  /* allocation failure */
                prime_list = tmp;
            }
            prime_list[prime_len++] = current_sum;
        }
    }

    return prime_list[n - 1];
}
