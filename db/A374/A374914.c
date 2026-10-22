/*
 * A374914  Primes p == 2, 3 (mod 4) with 2*p+1 prime.
 * https://oeis.org/A374914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime p ≡ 2 or 3 (mod 4) with 2p+1 prime (Sophie
 *            Germain primes of residue 2 or 3)
 * Range:     u8 n <= 6, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime_small.h"   /* bool is_prime_small(seq_t)  */
#include "../utils/is_prime_big.h"    /* bool is_prime_big(seq_t)    */

seq_t a(idx_t n)
{
    static seq_t *qual = NULL;     /* cache of qualifying primes */
    static size_t qual_cap = 0;    /* allocated capacity */
    static size_t qual_cnt = 0;    /* current number of stored primes */
    static idx_t last_checked = 2; /* last integer examined */

    /* Build until we have n terms in the cache */
    while (qual_cnt < n)
    {
        /* test primality of candidate prime */
        if (is_prime_small((seq_t)last_checked))
        {
            /* acceptable residues: 2 or 3 mod 4 (2 is the only even prime) */
            if ((last_checked & 3ULL) == 2ULL || (last_checked & 3ULL) == 3ULL)
            {
                seq_t cand = (seq_t)last_checked;
                seq_t q   = (cand << 1) + 1;        /* compute 2p+1 */

                if (is_prime_big(q))
                {
                    /* grow cache if necessary */
                    if (qual_cnt == qual_cap)
                    {
                        qual_cap = qual_cap ? qual_cap * 2 : 16;
                        qual = (seq_t *)realloc(qual,
                                                qual_cap * sizeof(seq_t));
                    }
                    qual[qual_cnt++] = (seq_t)last_checked;
                }
            }
        }
        ++last_checked;
    }

    /* Return the n‑th term (1‑indexed) */
    return qual[(size_t)n - 1];
}
