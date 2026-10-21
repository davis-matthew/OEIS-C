/*
 * A243915  a(n) = sigma(omega(n)).
 * https://oeis.org/A243915
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sigma(omega(n)) (sum of divisors of the number of
 *            distinct prime factors of n)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Input/Output types are supplied by the build system:
   idx_t  – unsigned 64‑bit integer (uint64_t)
   seq_t – arbitrary unsigned integer type (uint8_t … uint64_t or big integer) */

/* ------------------------------------------------------------------ */
/* Integer square root: floor(sqrt(n)) for 0 <= n <= 2^64-1          */
static uint64_t isqrt_u64(uint64_t n)
{
    if (n < 2) return n;
    uint64_t x = n >> 1;                 /* initial guess */
    uint64_t y = (x + n / x) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ------------------------------------------------------------------ */
/* Simple prime sieve – keeps a growing list of primes up to the       */
/* current required limit.  The list is stored in a static array.     */
static uint64_t *prime_list = NULL;        /* array of primes                        */
static size_t   prime_count = 0;           /* number of primes stored                */
static size_t   prime_cap   = 0;           /* allocated capacity                     */
static uint64_t prime_limit = 0;           /* largest prime in the list              */

static void grow_primes_to(uint64_t limit)
{
    if (limit <= prime_limit) return;      /* already covered                       */

    /* allocate space for new composites array */
    size_t sz = limit + 1;
    uint8_t *marked = (uint8_t*)calloc(sz, 1);   /* 0 = unmarked (prime)                   */
    if (!marked) exit(EXIT_FAILURE);         /* out of memory                          */

    /* sieve */
    for (uint64_t i = 2; i * i <= limit; ++i) {
        if (!marked[i]) {
            for (uint64_t j = i * i; j <= limit; j += i)
                marked[j] = 1;
        }
    }

    /* collect primes into the list */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!marked[i]) {
            /* enlarge array if necessary */
            if (prime_count == prime_cap) {
                size_t new_cap = prime_cap == 0 ? 16 : prime_cap * 2;
                uint64_t *tmp = (uint64_t*)realloc(prime_list, new_cap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                prime_list = tmp;
                prime_cap = new_cap;
            }
            prime_list[prime_count++] = i;
        }
    }

    free(marked);
    prime_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Count distinct prime factors (omega) of n (n >= 2).                  */
static unsigned int omega_of(uint64_t n)
{
    unsigned int distinct = 0;

    if (n < 2) return 0;    /* unused for the sequence (offset gives n>=2) */

    uint64_t temp = n;
    uint64_t sq = isqrt_u64(temp);
    grow_primes_to(sq);    /* ensure primes up to sqrt(n) are available */

    for (size_t i = 0; i < prime_count && prime_list[i] <= sq; ++i) {
        uint64_t p = prime_list[i];
        if (temp % p == 0) {
            ++distinct;
            while (temp % p == 0) temp /= p;
            sq = isqrt_u64(temp);
        }
    }

    if (temp > 1) ++distinct;       /* remaining prime factor > sqrt(original n) */

    return distinct;
}

/* ------------------------------------------------------------------ */
/* Sum of divisors of a small integer k (k <= number of distinct     */
/* prime factors; for n up to 10^4, k <= 13).                          */
static seq_t sigma_of_small(uint64_t k)
{
    seq_t sum = 0;
    for (uint64_t d = 1; d <= k; ++d)
        if (k % d == 0)
            sum += (seq_t)d;
    return sum;
}

/* ------------------------------------------------------------------ */
/* Main function required by the build system.                          */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                     /* values below the offset are irrelevant */

    unsigned int k = omega_of(n);            /* omega(n) */
    return sigma_of_small(k);                /* sigma(omega(n)) */
}
