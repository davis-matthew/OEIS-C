/*
 * A325421  Numbers k coprime to 10 such that there are exactly two values of
 *          A for which k^2+4*A and k^2-4*A are perfect squares.
 * https://oeis.org/A325421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth odd integer k, not divisible by 5, such that * k
 *            has exactly one prime factor p ≡ 1 (mod 4) with exponent 2, *
 *            and no other primes ≡ 1 (mod 4). All other prime factors (if
 *            any) * are ≡ 3 (mod 4). The sequence is the numbers k in
 *            increasing order
 * Range:     u8 n <= 1, u16 n <= 222, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- Prime table ---------- */
static uint64_t *prime_table = NULL;   /* array of primes  */
static size_t prime_count  = 0;        /* number of primes  */

/* Build all primes up to LIMIT (chosen large enough for the needed range). */
static void init_prime_table(void)
{
    if (prime_table != NULL)
        return;                /* already built */

    const idx_t LIMIT = 2000000U;  /* enough for all indices requested */
    /* Sieve of Eratosthenes */
    unsigned char *sieve = (unsigned char *)malloc((size_t)(LIMIT + 1));
    if (sieve == NULL)
        exit(EXIT_FAILURE);
    memset(sieve, 0, (size_t)(LIMIT + 1));

    for (idx_t i = 2; i * i <= LIMIT; ++i)
        if (!sieve[i])
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = 1;

    /* Count primes */
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            prime_count++;

    /* Allocate and fill prime array */
    prime_table = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (prime_table == NULL)
        exit(EXIT_FAILURE);
    size_t idx = 0;
    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i])
            prime_table[idx++] = (uint64_t)i;

    free(sieve);
}

/* ---------- Sequence storage ---------- */
static seq_t *seq_vals = NULL;   /* array of found terms          */
static size_t seq_len  = 0;      /* number of terms stored        */
static size_t seq_cap  = 0;      /* allocated capacity            */
static idx_t  last_checked = 1;  /* last candidate examined       */

/* Grow the array if necessary   */
static void ensure_seq_capacity(void)
{
    if (seq_len < seq_cap)
        return;
    size_t new_cap = (seq_cap == 0) ? 16 : seq_cap * 2;
    seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (new_arr == NULL)
        exit(EXIT_FAILURE);
    for (size_t i = 0; i < seq_len; ++i)
        new_arr[i] = seq_vals[i];
    if (seq_vals)
        free(seq_vals);
    seq_vals = new_arr;
    seq_cap  = new_cap;
}

/* Append a term to the sequence array   */
static void append_term(seq_t val)
{
    ensure_seq_capacity();
    seq_vals[seq_len++] = val;
}

/* ---------- Test if a candidate satisfies the condition ---------- */
static bool is_candidate(idx_t k)
{
    if ((k & 1U) == 0 || (k % 5U) == 0)
        return false;          /* must be odd and coprime to 10 */

    idx_t n = k;
    int one_mod4_primes = 0;   /* count of primes ≡1 mod4 found (must be exactly one) */

    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_table[i];
        if ((idx_t)p * (idx_t)p > n)
            break;
        if (n % p == 0) {
            int exp = 0;
            while (n % p == 0) {
                n /= p;
                exp++;
            }
            if ((p % 4U) == 1U) {
                if (one_mod4_primes != 0)
                    return false;        /* more than one prime ≡1 mod4 */
                if (exp != 2)
                    return false;        /* exponent must be exactly 2 */
                one_mod4_primes = 1;
            } else { /* p % 4 == 3 or p==2 (does not occur for odd k) */
                /* no restriction on exponents of primes ≡3 mod4 */
            }
        }
    }
    if (n > 1) {   /* remaining prime factor > sqrt(k) */
        uint64_t rem = n;
        if ((rem % 4U) == 1U) {
            if (one_mod4_primes != 0)
                return false;
            if (rem == 5U)
                return false;          /* k is divisible by 5, but already filtered */
            /* remaining prime ≡1 mod4 must have exponent 1, so fail */
            return false;
        }
        /* remaining prime ≡3 mod4 – ok */
    }
    return (one_mod4_primes == 1);
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0U)
        return (seq_t)0;          /* undefined for n=0, guard */

    init_prime_table();

    /* Generate terms until we have at least n terms */
    while (seq_len < (size_t)n) {
        /* Advance to next candidate (odd, not divisible by 5) */
        do {
            last_checked += 2;
        } while ((last_checked % 5U) == 0);

        if (is_candidate(last_checked))
            append_term((seq_t)last_checked);
    }

    return seq_vals[(size_t)n - 1];
}
