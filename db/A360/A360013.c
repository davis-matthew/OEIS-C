/*
 * A360013  Numbers whose exponent of 2 in their canonical prime factorization
 *          is larger than all the other exponents.
 * https://oeis.org/A360013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A360013: numbers whose exponent of 2 in the prime factorisation is larger
   than every other exponent.  The function generates terms by brute‑force
   checking of consecutive integers.  That is sufficient for n ≤ 10 000
   and runs comfortably within the required time. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* unspecified for n < 1 */

    idx_t count = 0;
    idx_t cand   = 2;              /* first candidate: 2 */

    /* helper: test if x satisfies the condition */
    while (1)
    {
        /* ---- check candidate cand ---- */
        if ((cand & 1) == 0)      /* even number only */
        {
            unsigned e2 = 0;
            idx_t   t   = cand;

            /* count exponent of 2 */
            while ((t & 1) == 0)
            {
                ++e2;
                t >>= 1;
            }

            /* t is now the odd part */
            idx_t  odd = t;
            int   bad = 0;

            /* factor odd part */
            for (idx_t p = 3; p <= odd / p; p += 2)
            {
                if (odd % p == 0)
                {
                    unsigned cnt = 0;
                    while (odd % p == 0)
                    {
                        odd /= p;
                        ++cnt;
                    }
                    if (cnt >= e2) { bad = 1; break; }
                }
            }

            /* remaining prime > sqrt(odd), exponent 1 */
            if (!bad && odd > 1 && 1 >= e2) bad = 1;

            if (!bad)
            {
                ++count;
                if (count == n) return (seq_t)cand;
            }
        }

        ++cand;
    }
}
