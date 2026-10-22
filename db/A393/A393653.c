/*
 * A393653  ISBN-10 numbers with the same check digit upon conversion to
 *          ISBN-13.
 * https://oeis.org/A393653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini
 * Formula:   a(n) = 10*n + ch, where ch is the common ISBN‑10 * and ISBN‑13
 *            check digit of the 9‑digit number n
 * Range:     u8 n <= 3, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   compute_value_for_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/compute_value_for_n.h"

static size_t cache_len = 0;
static seq_t cache[10001];          /* enough to store the first 10 000 terms */
static seq_t next_n = 0;            /* next 9‑digit number to test */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset‑free indices */

    /* Clamp to the maximum number we have prepared a static buffer for. */
    if (n > 10000)
        return 0;                    /* out of range for this implementation */

    while (cache_len < n) {
        int ok;
        seq_t val = compute_value_for_n(next_n, &ok);
        if (ok)
            cache[cache_len++] = val;
        ++next_n;
    }
    return cache[n - 1];
}
