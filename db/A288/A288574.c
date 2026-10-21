/*
 * A288574  Total number of distinct primes in all representations of 2*n+1 as
 *          a sum of 3 odd primes.
 * https://oeis.org/A288574
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum over representations of 2*n+1 as a sum of 3 odd
 *            primes * of the number of distinct primes in each representation
 * Range:     u8 n <= 85, u16 n <= 3310, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t *primes = NULL;      /* list of odd primes up to LIMIT */
static size_t   primes_cnt = 0;      /* number of primes in the list */
static uint8_t *prime_flag = NULL;   /* 1 if the index is prime, 0 otherwise */
static bool initialized = false;     /* init flag */

/* Initialise the sieve and the prime list once. */
static void init_primes(void)
{
    const idx_t LIMIT = 20001;        /* since 2*n+1 <= 20001 for n <= 10000 */

    /* Allocate and zero the flag array. */
    uint8_t *flag = (uint8_t*)calloc((size_t)(LIMIT + 1), 1);
    if (!flag) return;                /* allocation failed – leave uninitialised */

    /* Mark all numbers >= 2 as potential primes. */
    for (idx_t i = 2; i <= LIMIT; ++i)
        flag[i] = 1;
    flag[0] = flag[1] = 0;

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (flag[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                flag[j] = 0;
        }
    }

    /* Count odd primes (>=3). */
    size_t cnt = 0;
    for (idx_t i = 3; i <= LIMIT; i += 2)
        if (flag[i])
            ++cnt;

    /* Allocate the prime array. */
    uint64_t *p = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!p) {
        free(flag);
        return;
    }

    /* Fill the prime array. */
    size_t idx = 0;
    for (idx_t i = 3; i <= LIMIT; i += 2)
        if (flag[i])
            p[idx++] = i;

    /* Store static data. */
    primes      = p;
    primes_cnt  = cnt;
    prime_flag  = flag;
    initialized = true;
}

/* -------------------------------------------------------------------- */
/* Main sequence function                                               */
/* -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Lazily initialise the prime data. */
    if (!initialized) init_primes();
    if (!initialized) return 0;  /* if initialization failed */

    seq_t total = 0;
    const idx_t N = 2 * n + 1;    /* target odd number */

    /* Enumerate unordered triples p <= q <= r. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        const uint64_t p = primes[i];
        if (p > N) break;
        for (size_t j = i; j < primes_cnt; ++j) {
            const uint64_t q = primes[j];
            if (p + q > N) break;
            const idx_t r = N - p - q;   /* r is guaranteed <= N */
            if (r < q) break;           /* enforce q <= r for uniqueness */
            if (!prime_flag[r]) continue;/* r is not prime */

            /* Count distinct primes in this representation. */
            seq_t distinct;
            if (p == q && q == r)
                distinct = 1;          /* p+p+p */
            else if (p == q || q == r)
                distinct = 2;          /* p+p+q or p+q+q */
            else
                distinct = 3;          /* all different */

            total += distinct;
        }
    }
    return total;
}
