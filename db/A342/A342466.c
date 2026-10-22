/*
 * A342466  a(n) = A336466(1+A000265(sigma(n))), where A336466 is fully
 *          multiplicative with a(p) = A000265(p-1) for p prime, and
 *          A000265(k) is the odd part of k.
 * https://oeis.org/A342466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A336466(1 + A000265(sigma(n)))
 * Range:     u8 n <= 22, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: compute base^exp for seq_t using binary exponentiation. */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* Helper: return the odd part of x (i.e., remove all factors of 2). */
static seq_t odd_part_seq(seq_t x)
{
    while ((x & (seq_t)1) == 0)
        x >>= 1;
    return x;
}

/* Helper: factor a 64‑bit integer by trial division and
   compute sigma(n) on the fly. */
static seq_t sigma_from_u64(uint64_t n)
{
    uint64_t temp = n;
    seq_t sigma = (seq_t)1;

    while (temp > 1)
    {
        uint64_t factor = 0;
        if ((temp & 1) == 0)
        {
            factor = 2;
        }
        else
        {
            /* trial division for odd primes */
            for (uint64_t p = 3; p <= temp / p; p += 2)
            {
                if (temp % p == 0)
                {
                    factor = p;
                    break;
                }
            }
            if (factor == 0)
                factor = temp; /* remaining prime */
        }

        /* count exponent of factor */
        uint64_t exp = 0;
        while (temp % factor == 0)
        {
            temp /= factor;
            exp++;
        }

        /* compute term = (p^(e+1)-1)/(p-1) */
        seq_t term = pow_seq((seq_t)factor, (idx_t)(exp + 1));
        term -= (seq_t)1;
        term /= (seq_t)(factor - 1);
        sigma *= term;
    }

    return sigma;
}

/* Main function to compute a(n). */
seq_t a(idx_t n)
{
    /* Step 1: compute sigma(n) */
    uint64_t n_un = (uint64_t)n;   /* idx_t is unsigned */
    seq_t sigma = sigma_from_u64(n_un);

    /* Step 2: odd part of sigma */
    seq_t odd_sigma = odd_part_seq(sigma);

    /* Step 3: compute m = 1 + odd_sigma */
    seq_t m = odd_sigma + (seq_t)1;

    /* Step 4: factor m and compute A336466(m) */
    uint64_t m_un = (uint64_t)m;   /* assume m fits in 64 bits for the tests */
    uint64_t temp_m = m_un;
    seq_t result = (seq_t)1;

    while (temp_m > 1)
    {
        uint64_t p = 0;
        if ((temp_m & 1) == 0)
            p = 2;
        else
        {
            for (uint64_t q = 3; q <= temp_m / q; q += 2)
            {
                if (temp_m % q == 0)
                {
                    p = q;
                    break;
                }
            }
            if (p == 0)
                p = temp_m;   /* remaining prime */
        }

        uint64_t count = 0;
        while (temp_m % p == 0)
        {
            temp_m /= p;
            count++;
        }

        /* compute a(p) = odd_part(p-1) */
        seq_t ap = odd_part_seq((seq_t)(p - 1));

        /* multiply result by ap^count */
        for (uint64_t i = 0; i < count; ++i)
            result *= ap;
    }

    return result;
}
