/*
 * A259028  a(n) = least number m such that digital root of m is n*(digital
 *          root of m-th prime).
 * https://oeis.org/A259028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = least number m such that digital_root(m) = n *
 *            digital_root(prime(m))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digital_root_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          where prime(m) is the m‑th prime number.                                   */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/digital_root_seq.h"

/*----- dynamic list of primes -----*/
static uint32_t *primes      = NULL;   /* array of primes, primes[0] = 2   */
static size_t   primes_count = 0;      /* number of primes stored         */
static size_t   primes_cap   = 0;      /* allocated capacity              */
static uint32_t next_candidate = 2;    /* candidate for next prime test   */

/* Ensure at least `needed` primes are stored */
static void ensure_primes(size_t needed)
{
    while (primes_count < needed)
    {
        uint32_t cand = next_candidate;

        /* advance next candidate for future calls */
        if (cand <= 2)
            next_candidate = 3;
        else
            next_candidate = cand + 2;  /* skip even numbers */

        bool is_prime = true;
        for (size_t i = 0; i < primes_count; ++i)
        {
            uint32_t p = primes[i];
            if ((uint64_t)p * (uint64_t)p > cand)
                break;
            if (cand % p == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            if (primes_count == primes_cap)          /* grow storage */
            {
                size_t new_cap = primes_cap ? primes_cap * 2 : 8;
                uint32_t *tmp = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
                if (!tmp)
                    exit(EXIT_FAILURE);
                primes     = tmp;
                primes_cap = new_cap;
            }
            primes[primes_count++] = cand;
        }
    }
}

/*----- sequence function -----*/
seq_t a(idx_t n)
{
    if ((seq_t)n > (seq_t)9)   /* product n*dr(prime) would exceed 9, impossible */
        return (seq_t)0;       /* defined to be 0 for undefined terms */

    seq_t sn = (seq_t)n;
    idx_t m  = 1;                 /* candidate integer value and prime index */

    while (true)
    {
        ensure_primes((size_t)m);                   /* need prime(m)          */
        seq_t prime_m = (seq_t)primes[(size_t)m - 1];

        seq_t dr_m   = digital_root_seq((seq_t)m);
        seq_t dr_p   = digital_root_seq(prime_m);

        /* if the product exceeds 9, skip this m */
        if (sn > (seq_t)(9 / dr_p))
        {
            ++m;
            continue;
        }

        seq_t product = sn * dr_p;   /* guaranteed <= 9 */
        if (dr_m == product)
            return (seq_t)m;

        ++m;
    }
}
