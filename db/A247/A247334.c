/*
 * A247334  Highly abundant numbers which are not abundant.
 * https://oeis.org/A247334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   a(n) = the nth highly abundant integer with sigma(n) <= 2n. This
 *            function computes the sequence on demand by scanning integers,
 *            maintaining the running maximum of sigma, and storing those that
 *            satisfy the “not abundant” condition. The sequence has a finite
 *            number of terms (8 in total)
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Helper: sum of divisors (sigma function) using prime factorisation. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = 1;           /* start with divisor 1 */
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            seq_t p_pow = 1;
            seq_t total = 1;  /* 1 + p + p^2 + ... */
            while (m % p == 0)
            {
                m /= p;
                p_pow *= (seq_t)p;
                total += p_pow;
            }
            sum *= total;
        }
    }
    if (m > 1)                 /* remaining prime factor */
        sum *= (1 + (seq_t)m);
    return sum;
}

/* Cached term list and initialization flag. */
static seq_t terms[256];
static size_t terms_len = 0;
static int terms_initialized = 0;

/* Initialise the term list once, scanning up to a safe bound. */
static void init_terms(void)
{
    if (terms_initialized) return;
    terms_initialized = 1;
    terms_len = 0;

    seq_t max_sigma = 0;
    for (idx_t i = 1; i <= 500; ++i)          /* 500 is comfortably above 16 */
    {
        seq_t sigma = sigma_of(i);
        if (sigma > max_sigma)
        {
            max_sigma = sigma;
            if (sigma <= (seq_t)i * 2)        /* not abundant */
                terms[terms_len++] = (seq_t)i;
        }
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_terms();
    if (n == 0) return 0;                     /* offset is 1 */
    if ((size_t)(n - 1) < terms_len)          /* valid index */
        return terms[n - 1];
    return 0;                                 /* beyond sequence length */
}
