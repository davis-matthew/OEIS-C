/*
 * A301637  Primes of the form 4^k + k + 1.
 * https://oeis.org/A301637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^k + k + 1 (prime) for the n‑th such k, offset 1
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 2, u64 n <= 2, u128 n <= 2,
 *            big any n
 * Helpers:   big_pow, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* offset 1 – n should be >= 1 */
    if (n == 0) return (seq_t)0;

    static seq_t *primes = NULL;   /* cached primes */
    static size_t len   = 0;       /* number of primes cached */
    static size_t cap   = 0;       /* allocated capacity */
    static idx_t  next_k = 0;      /* next k to test */

    while (len < (size_t)n)
    {
        /* compute candidate = 4^k + k + 1 */
        seq_t val = big_pow((seq_t)4, (seq_t)next_k);
        val += (seq_t)next_k;
        val += (seq_t)1;

        if (is_prime_big(val))
        {
            if (len >= cap)
            {
                cap = (cap ? cap * 2 : 8);
                seq_t *tmp = (seq_t *)realloc(primes, cap * sizeof(seq_t));
                if (!tmp) abort();
                primes = tmp;
            }
            primes[len++] = val;
        }
        ++next_k;
    }

    return primes[n - 1];
}
