/*
 * A255964  Five-digit distinct-digit zeroless primes.
 * https://oeis.org/A255964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) is the n‑th 5‑digit prime whose digits are all non‑zero and
 *            pairwise distinct
 * Range:     u8 none, u16 n <= 1036, u32 n <= 1610, u64 n <= 1610,
 *            u128 n <= 1610, big any n
 * Helpers:   ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* offset 1 */

    /* cached primes */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t last_checked = 9999;  /* number just before the 5‑digit range */

    while (cache_len < n) {
        if (last_checked >= 99999)        /* no more 5‑digit numbers */
            return 0;

        ++last_checked;
        seq_t x = last_checked;

        /* all digits must be non‑zero and distinct */
        bool used[10] = {false};
        seq_t tmp = x;
        int digit_cnt = 0;
        bool ok = true;
        while (tmp > 0) {
            seq_t d_seq = tmp % 10;
            uint64_t d = (uint64_t)d_seq;   /* d ≤ 9, safe cast */

            if (d == 0 || used[(unsigned)d]) {
                ok = false;
                break;
            }
            used[(unsigned)d] = true;
            ++digit_cnt;
            tmp = tmp / 10;
        }
        if (!ok || digit_cnt != 5)
            continue;                    /* not a 5‑digit distinct‑digit number */

        /* primality test */
        if (!is_prime_small(x))
            continue;

        /* store prime in cache */
        if (cache_len + 1 > cache_cap) {
            ensure_capacity(cache_len + 1, &cache_cap,
                            (void **)&cache, sizeof(seq_t));
        }
        cache[cache_len++] = x;
    }

    return cache[n - 1];
}
