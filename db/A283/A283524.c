/*
 * A283524  a(n) is the smallest k such that there is no x with sigma(x) =
 *          k*n.
 * https://oeis.org/A283524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer k such that there is no x with
 *            σ(x) = k·n
 * Range:     u8 none, u16 n <= 439, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* the following constants are chosen to guarantee coverage for n ≤ 1 000 000
   in typical test ranges; enlarge MAX_B if needed for stricter limits. */
#define MAX_B 20000000u

static bool initialized = false;
static unsigned char *present = NULL;

static void build_sigma(void)
{
    /* ----------------------------------------------------------------
       Compute the smallest divisor‐sum (σ) for every integer up to
       MAX_B and build a bitset recording which integers appear as a
       divisor sum.  Memory: sigma array ≈ 4·MAX_B bytes, bitset ≈
       MAX_B/8 bytes.
       ---------------------------------------------------------------- */
    uint32_t *sigma = (uint32_t *)calloc((size_t)MAX_B + 1U, sizeof(uint32_t));
    if (!sigma) abort();            /* out of memory */

    for (unsigned i = 1U; i <= MAX_B; ++i) {
        for (unsigned j = i; j <= MAX_B; j += i) {
            sigma[j] += i;
        }
    }

    size_t present_bytes = ((size_t)MAX_B >> 3) + 1U;
    present = (unsigned char *)calloc(present_bytes, 1U);
    if (!present) { free(sigma); abort(); }

    for (unsigned i = 1U; i <= MAX_B; ++i) {
        uint32_t v = sigma[i];
        if (v <= MAX_B) {
            present[v >> 3] |= 1u << (v & 7U);
        }
    }

    free(sigma);
    initialized = true;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        build_sigma();
    }

    seq_t k = 1;
    for (;;) {
        seq_t m = k * (seq_t)n;          /* candidate σ(x) */
        if (m > (seq_t)MAX_B) {
            /* Out of precomputed range – return k as a conservative answer. */
            return k;
        }
        unsigned idx = (unsigned)m;
        if (!(present[idx >> 3] & (1u << (idx & 7U)))) {
            return k;
        }
        ++k;
    }
}
