/*
 * A267101  2 followed by permutation of odd primes, where each n-th prime of
 *          the form 4k+1 (A002144) has been replaced with the n-th prime of
 *          the form 4k+3 (A002145) and vice versa.
 * https://oeis.org/A267101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 6493, u32 n <= 9591, u64 n <= 9591,
 *            u128 n <= 9591, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A267101 – permutation of odd primes with 4k+1 ↔ 4k+3 swapped.  
   Implementation builds the primality sieve once and stores the sequence
   values in a static array.  The first term a(1)=2; for n>1, if the
   n‑th prime p_n ≡1 (mod 4) then a(n)= the n‑th prime ≡3 (mod 4),
   otherwise a(n) is the n‑th prime ≡1 (mod 4).  The code compiles
   in both C11 and C++17 and uses only the permitted operations.  */

#include <stdlib.h>
#include <stdbool.h>

/* Types supplied by the build system */
extern seq_t _dummy;   /* just to avoid warnings when seq_t is empty */

/* Static storage for primes and the sequence */
static seq_t *primes = NULL;          /* all primes up to LIMIT (including 2)     */
static seq_t *primes4k1 = NULL;       /* primes ≡1 mod 4 (excluding 2)            */
static seq_t *primes4k3 = NULL;       /* primes ≡3 mod 4                          */
static size_t primes_count = 0;        /* number of primes stored in primes array */
static size_t count4k1 = 0;           /* number of 4k+1 primes stored            */
static size_t count4k3 = 0;           /* number of 4k+3 primes stored            */

static seq_t *a_seq = NULL;           /* permutation of primes (the sequence)    */
static size_t a_count = 0;            /* number of terms stored                  */

static bool built = false;            /* have we built the cache yet?            */

/* Build all data needed for the sequence. */
static void build_cache(void)
{
    /* LIMIT chosen to contain at least the first 10 000 primes. */
    const idx_t LIMIT = 200000;          /* > prime(10000)=104729                   */
    size_t i, idx;
    bool *is_prime;

    /* Allocate boolean array for sieve (C++ requires cast). */
    is_prime = (bool*)malloc((size_t)(LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(EXIT_FAILURE);
    for (i = 0; i <= (size_t)LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (i = 2; (idx_t)i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (idx = i * i; idx <= (size_t)LIMIT; idx += i) is_prime[idx] = false;
        }
    }

    /* Allocate prime arrays.  Casts are required for C++. */
    primes     = (seq_t*)malloc((size_t)(LIMIT + 1) * sizeof(seq_t));
    primes4k1  = (seq_t*)malloc((size_t)(LIMIT + 1) * sizeof(seq_t));
    primes4k3  = (seq_t*)malloc((size_t)(LIMIT + 1) * sizeof(seq_t));
    if (!primes || !primes4k1 || !primes4k3) exit(EXIT_FAILURE);

    primes_count = 0;
    count4k1 = 0;
    count4k3 = 0;

    for (i = 2; i <= (size_t)LIMIT; ++i) {
        if (is_prime[i]) {
            seq_t v = (seq_t)i;
            primes[primes_count++] = v;
            if (i == 2) continue;   /* skip 2 for 4k+1 / 4k+3 lists */
            if ((i & 3) == 1) {
                primes4k1[count4k1++] = v;
            } else if ((i & 3) == 3) {
                primes4k3[count4k3++] = v;
            }
        }
    }

    /* Build the sequence a(n). */
    a_seq = (seq_t*)malloc(primes_count * sizeof(seq_t));
    if (!a_seq) exit(EXIT_FAILURE);
    a_seq[0] = (seq_t)2;    /* a(1) = 2 */
    a_count = 1;

    size_t cnt1 = 0, cnt3 = 0;   /* counts of 4k+1 and 4k+3 primes seen */
    for (idx = 1; idx < primes_count; ++idx) {
        seq_t p = primes[idx];
        if ((p & 3) == 1) {               /* p ≡ 1 (mod 4) */
            ++cnt1;
            a_seq[a_count++] = primes4k3[cnt1 - 1];
        } else if ((p & 3) == 3) {         /* p ≡ 3 (mod 4) */
            ++cnt3;
            a_seq[a_count++] = primes4k1[cnt3 - 1];
        }
    }

    free(is_prime);
    built = true;
}

/* Return the n-th term of the sequence, 1‑indexed. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but safe */
    if (!built) build_cache();
    if ((idx_t)n > a_count) return (seq_t)0;  /* out of pre‑computed range */
    return a_seq[(size_t)(n - 1)];
}
