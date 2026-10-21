/*
 * A296938  Rational primes that decompose in the field Q(sqrt(17)).
 * https://oeis.org/A296938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nth rational prime that decomposes in Q(sqrt(17)) * i.e.,
 *            primes p with p mod 17 ∈ {1,2,4,8,9,13,15,16}
 * Range:     u8 n <= 23, u16 n <= 3265, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static state to allow arbitrary order of calls */
    static seq_t *q = NULL;          /* array of qualifying primes */
    static size_t qlen = 0;           /* number of stored primes */
    static size_t qcap = 0;           /* allocated capacity */
    static idx_t next = 2;            /* next integer to test */

    /* Expand the stored list until it contains the n‑th element */
    while (qlen < (size_t)n)
    {
        /* find the next qualifying prime */
        for (;;)
        {
            if (is_prime(next))
            {
                idx_t r = next % 17;
                if (r == 1 || r == 2 || r == 4 || r == 8 ||
                    r == 9 || r == 13 || r == 15 || r == 16)
                {
                    /* ensure capacity */
                    if (qlen == qcap)
                    {
                        size_t new_cap = qcap ? qcap * 2 : 1;
                        q = seq_realloc(q, qcap, new_cap);
                        qcap = new_cap;
                    }
                    /* store the prime */
                    q[qlen++] = (seq_t)next;
                    ++next;              /* advance past this prime */
                    break;                /* back to outer while */
                }
            }
            ++next;                      /* test next integer */
        }
    }

    return q[n - 1U];
}
