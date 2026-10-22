/*
 * A371477  Positive integers that can be represented in an 8-bit floating
 *          point format.
 * https://oeis.org/A371477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A371477: positive integers representable as 1.M·2^(E–7) in
   an 8‑bit floating‑point format (E has 4 bits, M has 3 bits, sign 0).
   The set is finite (47 terms, largest 240).  The values are generated
   once and returned for any index n (1‑based). */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* 47 terms, indexed 1..47; vals[0] unused.  All values fit in
       seq_t, which is small enough for the computations (up to 240). */
    static seq_t vals[48] = {0};
    static bool inited = false;

    if (!inited) {
        size_t pos = 1;                  /* next free position, 1‑based */

        /* Exponent E ranges 0..14.  Set exp = E – 10 (because
           (8+M)·2^(E–10) gives the integer value).  exp goes from –3 to +4. */
        for (int exp = -3; exp <= 4; ++exp) {
            unsigned shift = (exp >= 0) ? (unsigned)exp : (unsigned)(-exp);
            for (int m = 0; m <= 7; ++m) {
                uint64_t base = 8u + (uint64_t)m;   /* 8–15 */
                if (exp < 0) {
                    /* Require base divisible by 2^shift. */
                    uint64_t mask = (1ULL << shift) - 1;
                    if ((base & mask) != 0) continue;   /* not divisible */
                    vals[pos++] = (seq_t)(base >> shift);
                } else {
                    vals[pos++] = (seq_t)(base << shift);
                }
            }
        }
        inited = true;   /* initialized once */
    }

    /* Bounds check: n must be between 1 and 47 inclusive. */
    if (n < 1 || n >= 48) return 0;
    return vals[(size_t)n];
}
