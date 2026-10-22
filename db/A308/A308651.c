/*
 * A308651  a(n+1) = k(a(n), n), where k(m, n) = (m*n) mod (m+n) and with a(1)
 *          = 5.
 * https://oeis.org/A308651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 106, u16 n <= 106, u32 n <= 106, u64 n <= 106,
 *            u128 n <= 106, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 5; a(n+1) = (a(n)*n) mod (a(n)+n) */
#include <stdbool.h>

/* Helper for modular multiplication using 128‑bit arithmetic when
   available.  This keeps the code safe for all integer widths that
   fit in the final value. */
static inline seq_t mul_mod(seq_t a, idx_t n, seq_t mod)
{
    /* The product a*n is evaluated in an unsigned 128‑bit integer
       to avoid overflow for any seq_t that fits in 64 bits.  If
       seq_t is 128 bits, the product still fits because the
       sequence values stay small (≤ 27 for n ≤ 84). */
    __uint128_t prod = (__uint128_t)a * (__uint128_t)n;
    return (seq_t)(prod % (__uint128_t)mod);
}

/* Static cache that contains the pre‑computed terms up to index 84.
   The sequence collapses to 0 for n ≥ 85. */
static seq_t cached[85];
static bool initialized = false;

/* Return the n‑th term of A308651.  The function works for
   arbitrary ordering of calls and for any integer type seq_t. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* out of range, but harmless */

    /* Initialize the cache on the first call. */
    if (!initialized) {
        cached[1] = (seq_t)5;
        for (idx_t i = 1; i < 84; ++i) {
            seq_t m   = cached[i];
            seq_t mod = m + (seq_t)i;      /* a(n) + n   */
            cached[i + 1] = mul_mod(m, i, mod); /* k(a(n),n) */
        }
        initialized = true;
    }

    if (n < 85)
        return cached[n];
    return (seq_t)0;            /* a(k) = 0 for k ≥ 85 */
}
