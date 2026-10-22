/*
 * A382027  Primes whose decimal digits are in ascending order and also parity
 *          alternating.
 * https://oeis.org/A382027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   cmp_seq_t, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes whose decimal digits are strictly increasing and whose parities
   alternate (odd‑even‑odd‑...). */
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cmp_seq_t.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Cache all terms – the whole set is finite (≤511). */
    static seq_t cache[512];
    static size_t cache_len = 0;
    static bool initialized = false;

    if (!initialized) {
        for (int mask = 1; mask < (1 << 9); ++mask) {
            /* Collect the digits in ascending order. */
            int digits[9];
            int dcnt = 0;
            for (int d = 1; d <= 9; ++d) {
                if (mask & (1 << (d - 1)))
                    digits[dcnt++] = d;
            }

            /* Check parity alternation. */
            bool ok = true;
            for (int i = 1; i < dcnt; ++i) {
                if ((digits[i] & 1) == (digits[i - 1] & 1)) {
                    ok = false;
                    break;
                }
            }
            if (!ok)
                continue;

            /* Build the integer value. */
            seq_t val = 0;
            for (int i = 0; i < dcnt; ++i)
                val = val * 10 + digits[i];

            /* Keep only primes. */
            if (is_prime_big(val))
                cache[cache_len++] = val;
        }

        /* Sort the found primes. */
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq_t);
        initialized = true;
    }

    if (n == 0 || n > cache_len)
        return (seq_t)0;

    return cache[n - 1];
}
