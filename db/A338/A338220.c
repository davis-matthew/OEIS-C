/*
 * A338220  Numbers k such that the Motzkin number A001006(k) is divisible by
 *          5.
 * https://oeis.org/A338220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 6554, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A338220: numbers k such that Motzkin number M(k) is divisible by 5. */

#include <stdbool.h>
#include <stdint.h>

/* Pre‑compute powers of 5 up to a safe limit for uint64_t */
#define MAX_POW_E 27
static idx_t pow5[MAX_POW_E + 1];
static bool pow5_inited = false;

static void init_pow5(void)
{
    if (pow5_inited) return;
    pow5[0] = (idx_t)1;
    for (idx_t e = 1; e <= MAX_POW_E; ++e) {
        pow5[e] = pow5[e-1] * (idx_t)5;
    }
    pow5_inited = true;
}

/* Check whether k satisfies one of the four Motzkin‑5 patterns. */
static bool is_divisible(idx_t k)
{
    init_pow5();
    const idx_t x1 = k + (idx_t)2; /* for patterns 1 & 3 */
    const idx_t x2 = k + (idx_t)1; /* for patterns 2 & 4 */

    for (idx_t e = 1; e <= MAX_POW_E; ++e) {
        const idx_t p = pow5[e];
        if (p > x1 && p > x2) break;

        if (x1 % p == 0) {
            const idx_t m = x1 / p;
            if ((e & 1) == 0 ? (m % 5 == 1) : (m % 5 == 3))
                return true;
        }
        if (x2 % p == 0) {
            const idx_t m = x2 / p;
            if ((e & 1) == 1 ? (m % 5 == 2) : (m % 5 == 4))
                return true;
        }
    }
    return false;
}

/* Return the nth term (1‑based) of A338220.  */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, but avoid UB */

    idx_t count = 0;
    idx_t k = 1;

    while (count < n) {
        if (is_divisible(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
        ++k;
    }

    /* Should never reach here */
    return (seq_t)0;
}
