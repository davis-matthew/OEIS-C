/*
 * A266123  Decrement by 1 all digits > 1 in factorial base representation of
 *          n and then shift it one digit right.
 * https://oeis.org/A266123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 580, u16 n <= 9800, u32 n <= 9800, u64 n <= 9800,
 *            u128 n <= 9800, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Replace each factorial base digit >1 with digit-1, then shift
   to the right (drop the least‑significant digit).  Implementation uses
   iterative factorial base conversion. */
#include <stdbool.h>

static seq_t fact[20];
static bool fact_initialized = false;

/* One-time factorial precomputation up to 19!. */
static void init_factorials(void) {
    if (fact_initialized) return;
    fact[0] = (seq_t)1;
    for (int i = 1; i < 20; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }
    fact_initialized = true;
}

seq_t a(idx_t n) {
    init_factorials();

    seq_t rem = (seq_t)n;
    seq_t result = 0;

    /* Find maximum k such that k! ≤ n.  For n < 10000 this is ≤ 9. */
    int k = 1;
    while (k + 1 < 20 && fact[k + 1] <= rem) {
        ++k;
    }

    for (int i = k; i >= 1; --i) {
        seq_t ai = rem / fact[i];
        rem = rem - ai * fact[i];
        seq_t ai_prime = (ai > 1) ? (ai - 1) : ai;
        if (i >= 2) {
            result += ai_prime * fact[i - 1];
        }
    }
    return result;
}
