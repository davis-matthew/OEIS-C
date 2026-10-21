/*
 * A270122  Indices of prime numbers of the form x^2 + x + x*y + y + y^2 where
 *          x and y are integers.
 * https://oeis.org/A270122
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the index of the nth prime p that can be written as p =
 *            x^2 + x + x*y + y + y^2 for some integers x and y
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

/* static data used by the sequence builder */
static idx_t  current_limit = 0;       /* current sieve limit */
static idx_t *prime_index  = NULL;     /* prime_index[v] = index of prime v (0 if not prime) */
static bool  *rep_flag     = NULL;     /* rep_flag[i] = true if prime index i appears in the sequence */
static seq_t *seq_values   = NULL;     /* sorted list of sequence values (prime indices) */
static size_t seq_count    = 0;        /* number of values stored in seq_values */

/* Forward declarations */
static void ensure_sequence(idx_t n);

/* Main entry point required by the problem */
seq_t a(idx_t n)
{
    ensure_sequence(n);
    return seq_values[n - 1];
}

/* ---------------------------------------------------------------------- */
/* Helper that builds the sequence until we have at least n terms.        */
/* ---------------------------------------------------------------------- */
static void ensure_sequence(idx_t n)
{
    if (seq_count >= (size_t)n)
        return;

    /* Increase sieve limit progressively until we have enough terms */
    size_t new_limit = current_limit ? current_limit * 2 : 200000;
    while (1) {
        size_t limit = new_limit;
        /* --- sieve primes up to limit --------------------------------- */
        bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
        if (!is_prime)
            exit(EXIT_FAILURE);
        for (size_t i = 2; i <= limit; ++i)
            is_prime[i] = true;
        for (size_t i = 2; i * i <= limit; ++i) {
            if (!is_prime[i])
                continue;
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = false;
        }

        /* --- build prime_index array --------------------------------- */
        if (prime_index)
            free(prime_index);
        prime_index = (idx_t *)calloc(limit + 1, sizeof(idx_t));
        if (!prime_index)
            exit(EXIT_FAILURE);
        idx_t pi = 1;
        for (size_t v = 2; v <= limit; ++v)
            if (is_prime[v])
                prime_index[v] = pi++;

        idx_t prime_count = pi - 1;

        /* --- allocate representation flag array --------------------- */
        if (rep_flag)
            free(rep_flag);
        rep_flag = (bool *)calloc(prime_count + 1, sizeof(bool));
        if (!rep_flag)
            exit(EXIT_FAILURE);

        /* --- search all pairs (x,y) that produce a prime value -------- */
        idx_t S = (idx_t)sqrt((double)limit) + 5;  /* safety margin */
        for (long long x = -(long long)S; x <= (long long)S; ++x) {
            for (long long y = -(long long)S; y <= (long long)S; ++y) {
                long long val = x * x + x * y + y * y + x + y;
                if (val < 2 || val > (long long)limit)
                    continue;
                size_t uv = (size_t)val;
                if (!is_prime[uv])
                    continue;
                idx_t idx = prime_index[uv];
                if (idx)
                    rep_flag[idx] = true;
            }
        }

        /* --- collect sequence values ---------------------------------- */
        if (seq_values)
            free(seq_values);
        seq_values = (seq_t *)malloc((prime_count + 1) * sizeof(seq_t));
        if (!seq_values)
            exit(EXIT_FAILURE);

        size_t found = 0;
        for (idx_t i = 1; i <= prime_count; ++i) {
            if (rep_flag[i])
                seq_values[found++] = (seq_t)i;
        }
        seq_count = found;
        current_limit = limit;

        /* --- cleanup ---------------------------------------------- */
        free(is_prime);

        if (seq_count >= (size_t)n)
            break;

        /* Increase limit and try again */
        new_limit *= 2;
    }
}
