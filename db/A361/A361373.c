/*
 * A361373  Number of prime powers p^m <= n such that p | n.
 * https://oeis.org/A361373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p | n} floor(log n / log p)
 * Range:     u8 n <= 467, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts, for each distinct prime divisor p of n, the number of
   positive integers m such that p^m ≤ n.  For a prime power n = p^e
   the result is simply e. */

/* The function below works for any idx_t and seq_t types provided
   by the build system.  It performs trial division up to sqrt(n) to
   find the distinct prime divisors of n and counts the admissible
   powers by repeated multiplication, avoiding overflow with a
   pre‑multiplcation check.  The result is returned in type seq_t. */

seq_t a(idx_t n)
{
    /* Handle the trivial case  n = 0 or 1  */
    if (n <= 1)
        return 0;

    seq_t result = 0;          /* running total of prime powers */
    idx_t temp   = n;          /* mutable copy for factorisation */
    seq_t sn     = n;          /* n cast to seq_t for comparisons */

    /* trial division over possible prime factors */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p != 0)      /* not a divisor */
            continue;

        /* p divides n – count admissible powers p^m ≤ n */
        seq_t count = 0;
        seq_t val   = (seq_t)p;  /* current power of p */

        /* multiply while staying ≤ n, using pre-check*/
        while (val <= sn)
        {
            ++count;
            /* if next multiplication would overflow or exceed n,
               terminate the loop */
            if (val > sn / (seq_t)p)
                break;
            val *= (seq_t)p;
        }

        result += count;        /* add contribution of this prime */

        /* remove all copies of p from temp for prime iteration */
        while (temp % p == 0)
            temp /= p;
    }

    /* If a prime factor > sqrt(n) remains, it is a single large prime */
    if (temp > 1)
    {
        seq_t count = 0;
        seq_t val   = (seq_t)temp;

        while (val <= sn)
        {
            ++count;
            if (val > sn / (seq_t)temp)
                break;
            val *= (seq_t)temp;
        }
        result += count;
    }

    return result;
}
