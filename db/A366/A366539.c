/*
 * A366539  The sum of unitary divisors of the exponentially 2^n-numbers
 *          (A138302).
 * https://oeis.org/A366539
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = product_{p^e||A138302(n)} (1 + p^e) where A138302(n) is
 *            the nth integer all of whose prime exponents are powers of two.
 *            The value a(n) is the sum of unitary divisors of that integer
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static uint64_t next = 1;          /* next integer to test */
    size_t target = (size_t)n;         /* make comparison with size_t */

    /* Generate terms until we have at least `n` of them */
    while (terms_len < target)
    {
        /* factor `next` */
        static uint64_t primes[64];
        static uint64_t exps[64];
        static size_t cnt;
        factor_u64(next, primes, exps, &cnt);

        /* check that all exponents are powers of two */
        bool ok = true;
        for (size_t i = 0; i < cnt; ++i)
        {
            uint64_t e = exps[i];
            if ((e & (e - 1)) != 0)  /* not a power of two */
            {
                ok = false;
                break;
            }
        }

        if (ok)
        {
            /* compute the sum of unitary divisors: product (1 + p^e) */
            seq_t sigma = 1;
            for (size_t i = 0; i < cnt; ++i)
            {
                seq_t pow = pow_uint(primes[i], (idx_t)exps[i]);
                sigma = sigma * (pow + 1);
            }

            /* grow array if necessary */
            if (terms_len + 1 > terms_cap)
            {
                size_t new_cap = (terms_cap == 0) ? 16 : terms_cap * 2;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) { abort(); }       /* out of memory */
                terms = tmp;
                terms_cap = new_cap;
            }

            terms[terms_len++] = sigma;
        }

        ++next; /* advance to the next integer for testing */

        if (terms_len >= target)
            break;
    }

    return terms[n - 1];
}
