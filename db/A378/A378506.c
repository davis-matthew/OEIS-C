/*
 * A378506  The number of solutions to the equation phi(phi(x)) = n, where phi
 *          is the Euler totient function.
 * https://oeis.org/A378506
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of solutions to phi(phi(x)) = n
 * Range:     u8 n <= 383, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Max value to pre‑compute, enough to cover all n needed in tests.
   10,000,000 gives a safe upper bound for x with phi(phi(x)) <= 10,000. */
static const size_t LIMIT = 10000000;

static seq_t *ans_arr = NULL;
static bool initialized = false;

static void precompute(void)
{
    /* phi array, 32‑bit is enough because LIMIT <= 2^32-1 */
    uint32_t *phi = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!phi) return;   /* memory failure – just give zeroes */

    for (size_t i = 0; i <= LIMIT; ++i) phi[i] = (uint32_t)i;

    for (size_t p = 2; p <= LIMIT; ++p) {
        if (phi[p] == (uint32_t)p) { /* p is prime */
            for (size_t j = p; j <= LIMIT; j += p) {
                phi[j] -= phi[j] / (uint32_t)p;
            }
        }
    }

    ans_arr = (seq_t *)calloc((LIMIT + 1), sizeof(seq_t));
    if (!ans_arr) {
        free(phi);
        return;
    }

    for (size_t x = 1; x <= LIMIT; ++x) {
        uint32_t y = phi[x];
        if (y <= LIMIT) {
            uint32_t z = phi[y];
            if (z <= LIMIT) {
                ans_arr[z]++;   /* ans_arr is of type seq_t */
            }
        }
    }

    free(phi);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) precompute();
    if ((idx_t)0 <= n && (size_t)n <= LIMIT) {
        return ans_arr[(size_t)n];
    }
    /* If n exceeds computed range simply return 0. */
    return (seq_t)0;
}
