/*
 * A305810  Filter sequence for a(Sophie Germain primes > 3) = constant
 *          sequences.
 * https://oeis.org/A305810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 270, u16 n <= 9794, u32 n <= 9794, u64 n <= 9794,
 *            u128 n <= 9794, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n)=n          (n<5); 
 *          a(n)=5         if n is a Sophie Germain prime >3;
 *          a(n)=n+1 - (# of SG primes >3 <= n)  otherwise.
 */
#include <stdlib.h>
#include <stdbool.h>

/* static tables for primality and SG‑prime prefix */
static bool *is_prime = NULL;
static seq_t *sg_prefix = NULL;
static idx_t sieve_limit = 0;

/* grow the sieve to cover all numbers up to 2*n+1 */
static void
ensure_sieve(idx_t n)
{
    idx_t need = 2*n + 1;
    if (need <= sieve_limit)
        return;

    /* allocate new tables */
    bool *new_prime = (bool *)malloc((size_t)(need + 1) * sizeof(bool));
    seq_t *new_pref = (seq_t *)malloc((size_t)(need + 1) * sizeof(seq_t));
    if (!new_prime || !new_pref) abort();

    /* initialise primality */
    for (idx_t i = 0; i <= need; ++i)
        new_prime[i] = true;
    new_prime[0] = new_prime[1] = false;

    /* sieve of Eratosthenes (non‑overflowing loop) */
    for (idx_t p = 2; p <= need / p; ++p) {
        if (new_prime[p]) {
            for (idx_t m = p*p; m <= need; m += p)
                new_prime[m] = false;
        }
    }

    /* prefix: number of SG primes >3 (i.e. i>3, i and 2*i+1 prime) */
    new_pref[0] = 0;
    for (idx_t i = 1; i <= need; ++i) {
        bool is_sg = false;
        if (i > 3 && new_prime[i]) {
            if (2*i + 1 <= need && new_prime[2*i + 1])
                is_sg = true;
        }
        new_pref[i] = new_pref[i-1];
        if (is_sg)
            new_pref[i] = new_pref[i] + (seq_t)1;
    }

    /* replace old tables */
    free(is_prime);
    free(sg_prefix);
    is_prime = new_prime;
    sg_prefix = new_pref;
    sieve_limit = need;
}

/* main sequence function */
seq_t
a(idx_t n)
{
    if (n < 5)
        return (seq_t)n;

    ensure_sieve(n);

    bool is_sg = is_prime[n] && (2*n + 1 <= sieve_limit) && is_prime[2*n + 1];
    if (is_sg)
        return (seq_t)5;

    seq_t cnt = sg_prefix[n];
    return ((seq_t)n + (seq_t)1) - cnt;
}
