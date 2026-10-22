/*
 * A373677  Last element of each maximal run of non-prime-powers.
 * https://oeis.org/A373677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A373677
   Formula: a(n) = last element of each maximal run of non‑prime‑powers,
   i.e. the n-th integer k (k ≥ 1) such that k is not a prime power and k+1 is a prime power. */

/*  The implementation builds a sieve of primes and marks all prime powers
    up to an increasing limit.  It then scans integers k ≥ 1 until the
    requested number of terms has been produced.  The sequence is memoised
    so that calls with arbitrary indices are handled efficiently. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* forward declaration of the sequence function (required by the test harness) */
seq_t a(idx_t n);

/* ------------------------------------------------------------ */
/*  Helper: integer square root for idx_t (no floating point)  */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 1;
    while ((r + 1) * (r + 1) <= x) ++r;
    return r;
}

/* ------------------------------------------------------------ */
/*  Sieve tables and dynamic growth                                 */
static uint8_t *prime_power_tbl = NULL;     /* prime_power_tbl[i] == 1  iff i is a prime power (>=2) */
static size_t prime_power_size = 0;         /* size of prime_power_tbl (number of elements) */

/* Rebuilds prime power table up to 'limit' (inclusive).  The function
   is used both for initial allocation and when the range must be
   enlarged.  It creates a new table and frees the old one. */
static void rebuild_prime_power_table(idx_t limit)
{
    size_t new_size = (size_t)(limit + 1);          /* include 0 … limit */
    uint8_t *new_tbl = (uint8_t *)calloc(new_size, 1);
    if (!new_tbl) abort();                         /* out of memory */

    /* sieve of Eratosthenes for primes up to 'limit' */
    uint8_t *is_prime = (uint8_t *)malloc(new_size);
    if (!is_prime) abort();
    for (size_t i = 0; i < new_size; ++i) is_prime[i] = 1;
    if (new_size > 0) is_prime[0] = 0;
    if (new_size > 1) is_prime[1] = 0;

    const idx_t sq = isqrt_u64(limit);
    for (idx_t i = 2; i <= sq; ++i)
    {
        if (is_prime[i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* mark prime powers (include primes themselves) */
    for (idx_t i = 2; i <= limit; ++i)
    {
        if (is_prime[i])
        {
            idx_t power = i;
            while (power <= limit)
            {
                new_tbl[power] = 1;
                /* avoid overflow: stop if next multiplication would exceed limit */
                if (power > (idx_t)(limit / i)) break;
                power *= i;
            }
        }
    }

    free(is_prime);
    free(prime_power_tbl);
    prime_power_tbl = new_tbl;
    prime_power_size = new_size;
}

/* Ensure that prime_power_tbl can index up to 'need' (inclusive).  If not,
   the table is recreated with a larger size (approximately doubled). */
static void ensure_prime_power_capacity(idx_t need)
{
    if ((size_t)need + 1 <= prime_power_size) return;

    idx_t new_limit = (prime_power_size > 0) ? prime_power_size * 2 : 256;
    while ((size_t)new_limit <= (size_t)need) new_limit *= 2;
    rebuild_prime_power_table(new_limit);
}

/* ------------------------------------------------------------ */
/*  Sequence state                                                     */
static seq_t *seq_values   = NULL;   /* array of computed terms          */
static size_t seq_len     = 0;       /* current number of terms stored    */
static idx_t  next_k      = 0;       /* next integer to examine (k)       */

/* Generate terms until we have at least 'n' terms stored. */
static void generate_terms_up_to(idx_t n)
{
    while (seq_len < n)
    {
        /* examine integer k = next_k + 1 */
        ++next_k;
        ensure_prime_power_capacity(next_k + 1);

        bool k_non_prime_power = !prime_power_tbl[next_k];
        bool k1_is_prime_power = prime_power_tbl[next_k + 1];

        if (k_non_prime_power && k1_is_prime_power)
        {
            /* store this k as a term */
            seq_values = (seq_t *)realloc(seq_values, sizeof(seq_t) * (seq_len + 1));
            if (!seq_values) abort();
            seq_values[seq_len++] = (seq_t)next_k;
        }
    }
}

/* ------------------------------------------------------------ */
/*  Public interface: returns the nth term (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;     /* undefined for n<1; return 0 for safety */

    generate_terms_up_to(n);
    return seq_values[n - 1];
}
