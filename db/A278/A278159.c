/*
 * A278159  Run length transform of primorials, A002110.
 * https://oeis.org/A278159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 126, u32 n <= 1022, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Run‑length transform of primorials (OEIS A278159)
   a(n) = ∏ primorial(k) over all runs of consecutive 1‑bits of n.  */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* First 64 primes – enough for any run length in a 64‑bit index. */
static const uint32_t small_primes[64] = {
    2,  3,  5,  7, 11, 13, 17, 19, 23, 29,
   31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
   73, 79, 83, 89, 97,101,103,107,109,113,
  127,131,137,139,149,151,157,163,167,173,
  179,181,191,193,197,199,211,223,227,229,
  233,239,241,251,257,263,269,271,277,281,
  283,293,307,311
};

static seq_t primorial_vals[65];   /* primorial_vals[0] = 1, primorial_vals[i] = product of first i primes */
static bool prim_initialized = false;

static void init_prim(void)
{
    if (prim_initialized) return;
    primorial_vals[0] = (seq_t)1;
    for (size_t i = 0; i < 64; ++i) {
        primorial_vals[i + 1] = primorial_vals[i] * (seq_t)small_primes[i];
    }
    prim_initialized = true;
}

seq_t a(idx_t n)
{
    if (!prim_initialized) init_prim();

    seq_t result = (seq_t)1;
    idx_t x = n;

    while (x != 0) {
        /* Skip trailing zeros */
        while ((x & (idx_t)1) == 0) {
            x >>= 1;
            if (x == 0) break;
        }
        if (x == 0) break;

        /* Count a run of 1‑bits */
        size_t run = 0;
        while ((x & (idx_t)1) != 0) {
            ++run;
            x >>= 1;
        }

        /* Multiply by the primorial for this run length */
        result = result * primorial_vals[run];
    }

    return result;
}
