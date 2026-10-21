/*
 * A258782  Nearest integer to log_2(n!).
 * https://oeis.org/A258782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(log_2(n!))
 * Range:     u8 n <= 57, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* Compute a(n) = nearest integer to log_2(n!). */
seq_t a(idx_t n)
{
    /* Special cases */
    if (n == 0 || n == 1)
        return (seq_t)0;

    /* Static cache of fixed‑point sums of log_2(k). */
    static uint64_t *pref = NULL;    /* pref[i] = Σ_{k=1..i} log2(k) * 2^32 (rounded) */
    static idx_t pref_cap = 0;       /* current allocated size */

    /* Grow cache if necessary. */
    if (n >= pref_cap) {
        idx_t new_cap = (pref_cap == 0) ? 1 : pref_cap * 2;
        if (new_cap <= n)
            new_cap = n + 1;
        uint64_t *new_ptr = (uint64_t *)realloc(pref, new_cap * sizeof(uint64_t));
        if (new_ptr == NULL) { /* allocation failure – abort */
            abort();
        }
        pref = new_ptr;
        /* Initialize from 0 up to new_cap-1 */
        if (pref_cap == 0) {
            pref[0] = 0ULL;
            pref_cap = 1;
        }
        for (idx_t i = pref_cap; i < new_cap; ++i) {
            if (i == 0) {
                pref[i] = 0ULL;
            } else {
                double lg = log2((double)i);
                uint64_t fixed = (uint64_t)llround(lg * (1ULL << 32));
                pref[i] = pref[i - 1] + fixed;
            }
        }
        pref_cap = new_cap;
    }

    /* Extract integer and fractional parts of the sum. */
    uint64_t total = pref[n];
    uint64_t integer_part = total >> 32;
    uint64_t frac_part = total & 0xFFFFFFFFULL;

    /* Round to nearest integer. */
    if (frac_part >= (1ULL << 31))
        integer_part += 1;

    return (seq_t)integer_part;
}
