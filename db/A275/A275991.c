/*
 * A275991  a(n) = prime(composite(n)) - prime(n).
 * https://oeis.org/A275991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(composite(n)) - prime(n)
 * Range:     u8 n <= 117, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool initialized = false;
static uint32_t *primes      = NULL;
static uint32_t *composites  = NULL;
static size_t    primes_cnt  = 0;
static size_t    composites_cnt = 0;

/* Build primes and composites up to a fixed limit sufficient for n <= 10000 */
static void init(void)
{
    if (initialized)
        return;
    initialized = true;

    /* Limit chosen so that composite(10000) < LIMIT and primes[LIMIT] exists */
    const int LIMIT = 200000;
    bool is_prime[LIMIT + 1];

    for (int i = 0; i <= LIMIT; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (int i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    primes      = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    composites  = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!primes || !composites) {
        /* Allocation failure – abort; this should never happen in tests */
        exit(EXIT_FAILURE);
    }

    for (int i = 2; i <= LIMIT; ++i) {
        if (is_prime[i]) {
            primes[primes_cnt++] = (uint32_t)i;
        } else if (i >= 4) {          /* composites start at 4 */
            composites[composites_cnt++] = (uint32_t)i;
        }
    }
}

/* External linkage: define the required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) {
        /* Undefined for index 0 under the OEIS offset */
        return 0;
    }

    init();

    /* Composite index is 1‑based, so components[(n‑1)] gives the nth composite number */
    if ((size_t)(n - 1) >= composites_cnt)
        return 0; /* safety guard for out‑of‑range n */

    uint32_t comp_val = composites[(size_t)(n - 1)];
    if (comp_val == 0 || (size_t)(comp_val - 1) >= primes_cnt)
        return 0; /* safety guard – should not happen for n <= 10000 */

    seq_t prime_of_composite = (seq_t)primes[(size_t)(comp_val - 1)];
    seq_t prime_n            = (seq_t)primes[(size_t)(n - 1)];

    return prime_of_composite - prime_n;
}
