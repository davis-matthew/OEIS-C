/*
 * A377897  Numbers k such that k + PrimePi(k) is even.
 * https://oeis.org/A377897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers k such that k + PrimePi(k) is even.
   The sequence is constructed by scanning all integers
   k = 1, 2, … and keeping those with (k + pi(k)) % 2 == 0,
   where pi(k) counts primes ≤ k.  */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *seq = NULL;          /* array of sequence terms          */
static size_t seq_sz = 0;          /* current number of terms stored   */
static size_t seq_cap = 0;         /* capacity of the seq array        */
static idx_t checked_upto = 0;     /* highest integer examined so far  */
static idx_t sieve_limit = 0;      /* highest index for which sieve is built */
static bool *is_prime = NULL;      /* sieve array (true = prime)       */
static idx_t *prime_pi = NULL;     /* pi values for every i ≤ sieve_limit */

/* Ensure that the sieve and pi array are computed up to at least `limit`. */
static void ensure_sieve(idx_t limit)
{
    if (limit <= sieve_limit)
        return;

    /* Free old arrays if they exist */
    free(is_prime);
    free(prime_pi);

    sieve_limit = limit;
    is_prime = (bool *)malloc((sieve_limit + 1) * sizeof(bool));
    prime_pi = (idx_t *)malloc((sieve_limit + 1) * sizeof(idx_t));

    if (!is_prime || !prime_pi)
        abort(); /* memory allocation failed */

    /* Sieve of Eratosthenes (simple, works for limits up to a few 10⁶) */
    for (idx_t i = 0; i <= sieve_limit; ++i)
        is_prime[i] = true;
    if (sieve_limit >= 0) is_prime[0] = false;
    if (sieve_limit >= 1) is_prime[1] = false;

    for (idx_t p = 2; p * p <= sieve_limit; ++p) {
        if (is_prime[p]) {
            for (idx_t j = p * p; j <= sieve_limit; j += p)
                is_prime[j] = false;
        }
    }

    /* Prefix sum of primes: pi[i] = number of primes ≤ i */
    prime_pi[0] = 0;
    for (idx_t i = 1; i <= sieve_limit; ++i)
        prime_pi[i] = prime_pi[i - 1] + (is_prime[i] ? 1 : 0);
}

/* Append a new term to the sequence array, resizing if necessary */
static void append_term(seq_t val)
{
    if (seq_sz == seq_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 64;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!seq)
            abort();
        seq_cap = new_cap;
    }
    seq[seq_sz++] = val;
}

/* Generate sequence terms until we have at least `target` elements. */
static void generate_up_to(idx_t target)
{
    if (seq_sz >= target)
        return;

    /* Estimate an upper bound for the number of integers we need to check.
       Since roughly half of the integers qualify, checking up to 3*target
       gives ample safety for the required range. */
    idx_t estimated_limit = checked_upto > target * 3 ? checked_upto : target * 3;
    ensure_sieve(estimated_limit);

    while (seq_sz < target) {
        ++checked_upto;
        if (checked_upto > sieve_limit)
            ensure_sieve(checked_upto * 2 + 10);

        idx_t pi_val = prime_pi[checked_upto];
        if (((checked_upto + pi_val) & 1) == 0)
            append_term((seq_t)checked_upto);
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n=0, but return 0 */

    generate_up_to(n);
    return seq[n - 1];
}
