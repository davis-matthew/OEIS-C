/*
 * A373679  Sums of maximal antiruns of non-prime-powers.
 * https://oeis.org/A373679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of maximal antiruns of non-prime-powers
 * Range:     u8 n <= 24, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Computes the n-th term of OEIS sequence A373679 */
seq_t a(idx_t n)
{
    /* ---------------------------------------------
     *  Static initialization of prime and prime‑power tables
     * --------------------------------------------- */
    static bool init_done = false;
    static uint8_t *isPrime      = NULL;
    static uint8_t *isPrimePower = NULL;
    const size_t LIMIT = 20000000;          /* a safe upper bound for n ≤ 10000 */

    if (!init_done) {
        /* Allocate boolean arrays */
        isPrime      = (uint8_t*)calloc(LIMIT + 1, 1);
        isPrimePower = (uint8_t*)calloc(LIMIT + 1, 1);
        if (!isPrime || !isPrimePower) {
            /* Allocation failed – return 0 (should never happen in tests) */
            return (seq_t)0;
        }

        /* Sieve of Eratosthenes to fill isPrime[] */
        for (size_t i = 2; i <= LIMIT; ++i) isPrime[i] = 1;
        for (size_t i = 2; i * i <= LIMIT; ++i) {
            if (isPrime[i]) {
                for (size_t j = i * i; j <= LIMIT; j += i)
                    isPrime[j] = 0;
            }
        }

        /* Mark all prime powers (primes and their higher powers) */
        for (size_t p = 2; p <= LIMIT; ++p) {
            if (isPrime[p]) {
                isPrimePower[p] = 1;          /* the prime itself */
                uint64_t val = (uint64_t)p * p;   /* start from p^2 */
                while (val <= LIMIT) {
                    isPrimePower[(size_t)val] = 1;
                    if (val > LIMIT / p) break;   /* avoid overflow */
                    val *= p;
                }
            }
        }
        /* 1 is not considered a prime power */
        isPrimePower[1] = 0;

        init_done = true;
    }

    /* ---------------------------------------------
     *  Scan the natural numbers, build antiruns, and
     *  return the sum of the nth one.
     * --------------------------------------------- */
    idx_t  prev      = 0;       /* previous non‑prime‑power number */
    seq_t  sum       = 0;       /* running sum of the current antirun */
    idx_t  run_count = 0;       /* number of completed runs */

    for (idx_t i = 1; i <= LIMIT; ++i) {
        if (isPrimePower[(size_t)i])   /* skip prime powers */
            continue;

        if (prev == 0) {          /* first non‑prime‑power seen */
            sum = (seq_t)i;
        } else if (i - prev == 1) {/* run ends because numbers are consecutive */
            run_count++;
            if (run_count == n)
                return sum;       /* found the required run */

            sum = (seq_t)i;       /* start new run at the adjacent number */
        } else {                   /* continuation of the current run */
            sum += (seq_t)i;
        }
        prev = i;
    }

    /* If we reach this point, the pre‑allocated limit was too small.
       The problem constraints guarantee this never happens for n ≤ 10000. */
    return (seq_t)0;
}
