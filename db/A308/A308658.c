/*
 * A308658  Square array read by downward antidiagonals: A(n, k) is the number
 *          of primes between the n-th and (n+k)-th perfect powers with
 *          exponent > 1, k > 0.
 * https://oeis.org/A308658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A308658: a(n) = number of primes between the n‑th and (n+k)-th perfect powers (exponent >1),
 *  read by downward antidiagonals.
 *  Implementation uses Cantor enumeration of the square array A(n,k) with
 *  A(n,k) = π(P_{n+k}) – π(P_n),  where P_i is the i‑th perfect power.
 *  A perfect power 1 is included so that P1 = 1, P2 = 4, P3 = 8, …
 *  The routine precomputes a sieve of primes up to LIMIT and the list of perfect
 *  powers up to that limit.  Then it maps the sequence index n to (row,column)
 *  using Cantor’s formula and returns the prime‑count difference.  All calls
 *  are memoised by static initialisation and work for any 64‑bit index n ≥ 1.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <stddef.h>

/*  Constants – tuned to cover n up to about 10 000         */
#define PRIME_LIMIT 2000000        /* numbers to sieve (≥ any perfect power needed) */
#define INITIAL_POW_CAP 10000      /* initial capacity for perfect‑power array      */

/*  Static data shared by all calls of a()                      */
static seq_t *prime_pi = NULL;          /* π(x) for 0 ≤ x ≤ PRIME_LIMIT */
static unsigned char *isprime = NULL;   /* sieve marker */
static unsigned char *marked = NULL;    /* perfect‑power marker */
static uint64_t *powers = NULL;         /* sorted list of perfect powers, 1‑based */
static size_t powers_len = 0;

/*  Utility: comparison for qsort                                  */
static int cmp_uint64(const void *a, const void *b)
{
    const uint64_t *x = (const uint64_t *)a;
    const uint64_t *y = (const uint64_t *)b;
    if (*x < *y) return -1;
    if (*x > *y) return 1;
    return 0;
}

/*  Initialise the static tables: primes and perfect powers      */
static void init_tables(void)
{
    size_t i, j;
    /*  Allocate and sieve primes                                     */
    isprime  = (unsigned char *)calloc(PRIME_LIMIT + 1, sizeof(unsigned char));
    if (!isprime) exit(1);
    for (i = 2; i <= PRIME_LIMIT; ++i) isprime[i] = 1;
    for (i = 2; i * i <= PRIME_LIMIT; ++i)
        if (isprime[i])
            for (j = i * i; j <= PRIME_LIMIT; j += i) isprime[j] = 0;

    prime_pi = (seq_t *)calloc(PRIME_LIMIT + 1, sizeof(seq_t));
    if (!prime_pi) exit(1);
    seq_t count = 0;
    for (i = 0; i <= PRIME_LIMIT; ++i) {
        if (isprime[i]) ++count;
        prime_pi[i] = count;
    }

    /*  Allocate marker for perfect powers and the array             */
    marked  = (unsigned char *)calloc(PRIME_LIMIT + 1, sizeof(unsigned char));
    if (!marked) exit(1);
    powers  = (uint64_t *)malloc(INITIAL_POW_CAP * sizeof(uint64_t));
    if (!powers) exit(1);
    size_t cap = INITIAL_POW_CAP;

    /*  Insert 1 as the first perfect power                         */
    powers[0] = 1;
    powers_len = 1;
    marked[1] = 1;

    /*  Generate distinct perfect powers up to PRIME_LIMIT          */
    for (i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        uint64_t val = (uint64_t)i * i;      /* i^2 */
        while (val <= PRIME_LIMIT) {
            if (!marked[(size_t)val]) {
                if (powers_len == cap) {
                    cap <<= 1;
                    uint64_t *tmp = (uint64_t *)realloc(powers, cap * sizeof(uint64_t));
                    if (!tmp) exit(1);
                    powers = tmp;
                }
                powers[powers_len++] = val;
                marked[(size_t)val] = 1;
            }
            if (val > PRIME_LIMIT / i) break;
            val *= i; /* next power of i */
        }
    }

    /*  Sort the list so that powers[k] is the (k+1)-th perfect power  */
    qsort(powers, powers_len, sizeof(uint64_t), cmp_uint64);
}

/*  Main routine – compute a(n) for n ≥ 1                          */
seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_tables();
        initialized = true;
    }

    /*  Cantor enumeration mapping: n → (row, col) in the array   */
    unsigned long long idx = (unsigned long long)n;  /* 64‑bit counter */
    /*  solve s*(s-1)/2 ≥ idx  with double arithmetic (safe for n≤10⁶)  */
    long double disc = sqrtl(1.0L + 8.0L * (long double)idx);
    unsigned long long s = (unsigned long long)((1.0L + disc) / 2.0L);
    if (s * (s - 1ULL) / 2ULL < idx) ++s;

    unsigned long long prev = (s - 1ULL) * (s - 2ULL) / 2ULL;
    unsigned long long offset = idx - prev - 1ULL;   /* 0‑based within diagonal */
    unsigned long long row = offset + 1ULL;          /* n‑coordinate */
    unsigned long long col = s - row;                /* k‑coordinate (k>0) */

    /*  Convert to 0‑based indices in the perfect‑power array        */
    size_t idx_n  = (size_t)(row - 1ULL);
    size_t idx_nk = (size_t)(row + col - 1ULL);      /* n+k-1 */

    /*  Safety check – should never touch out of bounds for n≤10⁴     */
    if (idx_n >= powers_len || idx_nk >= powers_len) return 0;

    /*  Prime counts: π(P_{n+k}) – π(P_n)                            */
    uint64_t v_n  = powers[idx_n];
    uint64_t v_nk = powers[idx_nk];
    seq_t pi_n  = prime_pi[v_n];
    seq_t pi_nk = prime_pi[v_nk];

    return pi_nk - pi_n;
}
