/*
 * A360452  Number of fractions c/d with |c| <= d <= 2n and odd denominator
 *          when factors of 2 are canceled.
 * https://oeis.org/A360452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{odd d ≤ 2n} φ(d) + 1 for n > 0 a(0) = 0 where φ(d)
 *            is Euler's totient function. The algorithm precomputes φ(d) and
 *            its prefix sums over odd d up to 2 · max_n ( ≤ 20000 for the
 *            constraints )
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   // for size_t
#include <stdint.h>   // for uint64_t
#include <stdlib.h>   // for malloc, free

/* Maximum n expected in the test harness (≈10000). */
#define MAX_N       10000UL
#define MAX_LIMIT   (2 * MAX_N)

/* Dynamically allocated tables (only allocated once). */
static uint64_t   *phi_tbl      = NULL;  /* φ(d) for 0 … MAX_LIMIT */
static uint64_t   *odd_prefix   = NULL;  /* prefix sums of φ over odd d */

/* Initialisation flag. */
static int  initialized = 0;

/* Compute φ(d) for all 0 ≤ d ≤ MAX_LIMIT and the odd‑denominator
   prefix sums. */
static void
precompute(void)
{
    if (initialized)
        return;

    /* Allocate memory. */
    phi_tbl = (uint64_t *)malloc((MAX_LIMIT + 1) * sizeof(uint64_t));
    odd_prefix = (uint64_t *)malloc((MAX_LIMIT + 1) * sizeof(uint64_t));

    /* Initialise φ to the identity. */
    for (idx_t i = 0; i <= MAX_LIMIT; ++i)
        phi_tbl[i] = (uint64_t)i;

    /* Sieve: for each prime p, subtract φ[j]/p from φ[j]. */
    for (idx_t p = 2; p <= MAX_LIMIT; ++p) {
        if (phi_tbl[p] == (uint64_t)p) {            /* p is prime */
            for (idx_t j = p; j <= MAX_LIMIT; j += p) {
                phi_tbl[j] -= phi_tbl[j] / (uint64_t)p;
            }
        }
    }

    /* Prefix sums over odd d. */
    odd_prefix[0] = 0;
    for (idx_t i = 1; i <= MAX_LIMIT; ++i) {
        odd_prefix[i] = odd_prefix[i - 1] +
                        ((i & 1) ? phi_tbl[i] : 0);
    }

    initialized = 1;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    precompute();

    /* 2n is guaranteed ≤ MAX_LIMIT for the problem constraints. */
    idx_t limit = 2 * n;
    if (limit > MAX_LIMIT)
        limit = MAX_LIMIT;

    seq_t sum = (seq_t)odd_prefix[limit];
    return (seq_t)2 * sum + (seq_t)1;
}
