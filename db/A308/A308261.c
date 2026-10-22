/*
 * A308261  For any integer n, let d(n) be the smallest k > 0 such that at
 *          least one of n-k or n+k is a prime number; we build an undirected
 *          graph G on top of the prime numbers as follows: two consecutive
 *          prime numbers p and q are connected iff at least one of d(p) or
 *          d(q) equals q-p; a(n) is the number of t...
 * https://oeis.org/A308261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of primes in the n-th connected component of
 *            the graph G where vertices are consecutive primes p and q are
 *            connected iff min(p-prev, next-p) equals q-p. The component
 *            containing the smallest prime 2 is component 1, etc
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static bool *is_prime      = NULL;
static uint32_t *primes    = NULL;
static uint32_t *dmin      = NULL;
static size_t   prime_cnt  = 0;
static bool     initialized = false;

/* ------------------------------------------------------------------ */
/* Simple sieve and preprocessing of primes and their nearest‑prime
   distances.  The limit is chosen to provide many components (over
   100 000 primaries).  This value is sufficient for n up to 10 000.  */
/* ------------------------------------------------------------------ */
static void init_primes(void)
{
    const size_t LIMIT = 5000000;          /* 5 million.  Adjust if needed. */
    size_t i, j;

    is_prime = (bool*)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);
    memset(is_prime, true, LIMIT + 1);
    is_prime[0] = is_prime[1] = false;

    for (i = 2; i * i <= LIMIT; ++i)
        if (is_prime[i])
            for (j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;

    /* Count primes to allocate array. */
    prime_cnt = 0;
    for (i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            ++prime_cnt;

    primes = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) exit(1);

    size_t idx = 0;
    for (i = 2; i <= LIMIT; ++i)
        if (is_prime[i])
            primes[idx++] = (uint32_t)i;

    /* Compute nearest‑prime distance for each prime. */
    dmin = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    if (!dmin) exit(1);

    for (i = 0; i < prime_cnt; ++i) {
        uint32_t d_left  = (i > 0)     ? (primes[i] - primes[i-1]) : UINT32_MAX;
        uint32_t d_right = (i+1 < prime_cnt) ? (primes[i+1] - primes[i]) : UINT32_MAX;
        dmin[i] = (d_left < d_right) ? d_left : d_right;
    }

    initialized = true;
}

/* ------------------------------------------------------------------ */
/*   Sequence function.  Computes the size of the n‑th component.     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (!initialized) init_primes();

    if (n == 0) return 0;   /* offset is 1 */

    size_t comp = 0;        /* current component number (1‑based) */
    size_t i    = 0;        /* index into primes array */

    while (i < prime_cnt) {
        /* Start a new component.                     */
        size_t comp_len = 1; /* at least the starting prime */

        /* Extend component while consecutive primes are linked. */
        while (i + 1 < prime_cnt) {
            uint32_t p    = primes[i];
            uint32_t q    = primes[i+1];
            uint32_t diff = q - p;

            if (diff == dmin[i] || diff == dmin[i+1]) {
                ++comp_len;
                ++i;      /* move to next prime in the component */
            } else {
                break;    /* component ends */
            }
        }

        /* Finished one component. */
        ++comp;
        if (comp == (size_t)n)
            return (seq_t)comp_len;  /* cast to seq_t (built‑in or class) */

        ++i; /* move to the first prime after the component */
    }

    /* If n is larger than the number of components found, result is undefined. */
    return 0;
}
