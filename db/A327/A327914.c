/*
 * A327914  The 58 prime dates of each non-leap year of the form
 *          concatenate(month,day) with leading zero for days 1..9.
 * https://oeis.org/A327914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, fini, full, changed
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence a(n) consists of the prime numbers obtained by
   concatenating the month and the day of a non‑leap year, with a
   leading zero for days 1…9.  For each month m = 1…12 and for each
   legal day d of that month, form the integer 100·m + d and test
   for primality.  The terms are listed in increasing order; there
   are 58 such primes. */

#include <stdbool.h>

/* Helper: test primality for numbers <= 2000 (fits in 32 bits). */
static bool is_prime_small(unsigned int x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (unsigned int i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* Return the nth term of A327914 (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Number of prime dates in a non‑leap year. */
    enum { COUNT = 58 };

    /* Static cache of the 58 primes, lazy initialized. */
    static unsigned int primes[COUNT];
    static bool init = false;

    if (!init) {
        /* Days per month in a non‑leap year. */
        static const unsigned int month_len[12] = {
            31, 28, 31, 30, 31, 30,
            31, 31, 30, 31, 30, 31
        };
        unsigned int pos = 0;
        for (unsigned int m = 1; m <= 12 && pos < COUNT; ++m) {
            unsigned int max_day = month_len[m-1];
            for (unsigned int d = 1; d <= max_day && pos < COUNT; ++d) {
                unsigned int num = 100 * m + d;  /* concatenate(m, d) */
                if (is_prime_small(num))
                    primes[pos++] = num;
            }
        }
        init = true;
    }

    if (n == 0 || n > COUNT) return (seq_t)0;  /* out of range */
    return (seq_t)primes[(unsigned int)(n - 1)];
}
