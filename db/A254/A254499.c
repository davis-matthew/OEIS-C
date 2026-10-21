/*
 * A254499  Amicable factorions.
 * https://oeis.org/A254499
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   sum_fact_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes all amicable factorions by exhaustive search up to a fixed limit. */
#include <stddef.h>
#include "../utils/sum_fact_digits.h"

/* Increase this limit only if new amicable factorions are discovered. */
#define LIMIT 200000

static seq_t amicable_arr[200];
static size_t amicable_len = 0;
static int initialized = 0;

static void init_sequence(void) {
    if (initialized) return;

    /* Pre‑compute factorials of 0–9 in seq_t. */
    seq_t fact[10];
    fact[0] = (seq_t)1;
    for (int i = 1; i < 10; ++i) fact[i] = fact[i-1] * (seq_t)(i);

    /* Search for all amicable factorions up to LIMIT. */
    for (idx_t n = 1; n <= LIMIT; ++n) {
        seq_t f1 = sum_fact_digits(n, fact);
        idx_t f1_int = (idx_t)f1;               /* safe: f1 < 10⁶ for n ≤ LIMIT */
        seq_t f2 = sum_fact_digits(f1_int, fact);
        if (f2 == (seq_t)n) {
            if (amicable_len < 200) {
                amicable_arr[amicable_len++] = (seq_t)n;
            }
        }
    }

    initialized = 1;
}

seq_t a(idx_t n) {
    init_sequence();
    if (n == 0 || n > amicable_len) return 0;
    return amicable_arr[n - 1];
}
