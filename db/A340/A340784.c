/*
 * A340784  Heinz numbers of even-length integer partitions of even numbers.
 * https://oeis.org/A340784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer m whose prime factorisation satisfies
 *            (1) the total number of prime factors (with multiplicity) is
 *            even; (2) the sum of the exponents of primes whose 1‑based index
 *            in the prime list is odd is even. This is equivalent to the
 *            definition of A340784
 * Range:     u8 n <= 67, u16 n <= 9645, u32 n <= 9645, u64 n <= 9645,
 *            u128 n <= 9645, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Prime sieve – pre‑computed up to a fixed limit to avoid repeated
   expansions during sequence generation.  The limit is chosen large
   enough (~10 million) to cover the 10 000‑th term with room to spare. */
static const size_t SIEVE_LIMIT = 10000000;   /* 10 million */

/* Global state for the sieve */
static bool *is_composite = NULL;          /* is_composite[x] = true iff x composite */
static unsigned char *prime_parity = NULL; /* 1 if prime's index is odd, else 0 */
static unsigned int *prime_list = NULL;    /* list of primes in order */
static size_t prime_count = 0;             /* how many primes found */
static size_t prime_capacity = 0;          /* allocated capacity for prime_list */

/* Initialise the sieve once.  The casts are required for C++ compatibility. */
static void init_sieve(void)
{
    if (is_composite != NULL) return; /* already initialized */

    size_t cap = SIEVE_LIMIT + 1;
    is_composite     = (bool*)   calloc(cap, sizeof(bool));
    prime_parity     = (unsigned char*)calloc(cap, sizeof(unsigned char));
    prime_capacity   = 200000;              /* initial capacity for prime_list */
    prime_list       = (unsigned int*)malloc(prime_capacity * sizeof(unsigned int));

    for (size_t i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            /* prime found */
            if (prime_count == prime_capacity) {
                prime_capacity *= 2;
                prime_list = (unsigned int*)realloc(prime_list,
                                                    prime_capacity * sizeof(unsigned int));
            }
            /* set parity after incrementing the count – 1‑based index */
            prime_count += 1;
            prime_parity[i] = (unsigned char)((prime_count & 1) ? 1 : 0);

            prime_list[prime_count - 1] = (unsigned int)i;

            if (i <= SIEVE_LIMIT / i) {
                for (size_t j = i * i; j <= SIEVE_LIMIT; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
}

/* ------------------------------------------------------------------ */
/* Sequence cache – grows to accommodate any requested index. */
static seq_t *seq_cache = NULL;   /* allocated array for sequence values */
static size_t cache_capacity = 0; /* allocated size of seq_cache */
static idx_t cache_len = 0;       /* how many terms currently cached */
static uint64_t next_scan = 1;    /* next integer to test for membership */

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    /* Expand cache storage if required */
    if ((size_t)n > cache_capacity) {
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 256;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_cache = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        cache_capacity = new_cap;
    }

    /* Populate sequence up to n */
    while (cache_len < n) {
        /* Special case: 1 is always in the sequence */
        if (next_scan == 1) {
            seq_cache[cache_len++] = (seq_t)1;
            ++next_scan;
            continue;
        }

        uint64_t m = next_scan;
        uint64_t t = m;
        unsigned char total_parity = 0; /* parity of total exponent sum          */
        unsigned char odd_parity   = 0; /* parity of exponents for odd‑index primes */

        /* Factorise t using the precomputed primes */
        for (size_t k = 0; k < prime_count; ++k) {
            unsigned int p = prime_list[k];
            uint64_t pp = (uint64_t)p;
            if (pp * pp > t) break;
            if (t % pp == 0) {
                unsigned char epar = 0;
                while (t % pp == 0) {
                    t /= pp;
                    epar ^= 1; /* toggle parity per occurrence */
                }
                total_parity ^= epar;
                if (prime_parity[p]) odd_parity ^= epar;
            }
        }

        if (t > 1) { /* remaining factor is a prime > sqrt(m) */
            unsigned int p = (unsigned int)t;
            unsigned char epar = 1; /* exponent 1 */
            total_parity ^= epar;
            if (p <= SIEVE_LIMIT) {
                if (prime_parity[p]) odd_parity ^= epar;
            } else {
                /* If the prime is beyond the sieve limit (unlikely for requested n),
                   we approximate its parity by counting primes up to p: */
                size_t idx = 0;
                for (size_t i = 0; i < prime_count; ++i) {
                    if (prime_list[i] <= p) ++idx;
                    else break;
                }
                if ((idx & 1) != 0) odd_parity ^= epar;
            }
        }

        /* Membership test: both parities must be zero (even). */
        if ((total_parity == 0) && (odd_parity == 0)) {
            seq_cache[cache_len++] = (seq_t)next_scan;
        }

        ++next_scan;
    }
}

/* ------------------------------------------------------------------ */
/* Public API – returns the n‑th term of A340784, for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1 */
    if (!is_composite) init_sieve();
    ensure_cache(n);
    return seq_cache[n - 1];
}
