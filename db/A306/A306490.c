/*
 * A306490  Numbers k such that sigma(k) - k - 2 is prime.
 * https://oeis.org/A306490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th integer k (k ≥ 1) such that sigma(k) – k – 2 is
 *            a prime number, where sigma(k) is the sum of all positive
 *            divisors of k. The computation is performed by searching
 *            integers in increasing order and collecting those that satisfy
 *            the condition. Results are cached in a static array so...
 * Range:     u8 n <= 26, u16 n <= 3414, u32 n <= 3414, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/sigma_of.h"   /* seq_t sigma_of(idx_t n);  */
#include "../utils/is_prime_big.h"  /* bool is_prime_big(seq_t n); */

static seq_t *collected = NULL;   /* stored valid k's          */
static size_t collected_len = 0;
static size_t collected_cap = 0;
static idx_t last_checked = 0;    /* last k processed          */

/* grow the static array to hold at least `min_cap` elements */
static void ensure_collected(void)
{
    if (collected_cap == 0) {
        collected_cap = 16;
        collected = (seq_t *)calloc(collected_cap, sizeof(seq_t));
        if (!collected) exit(EXIT_FAILURE);
    } else if (collected_len == collected_cap) {
        collected_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(collected, collected_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        collected = tmp;
    }
}

seq_t a(idx_t n)
{
    /* offset is 1 – there is no defined term for n = 0 */
    if (n == 0) return 0;

    /* keep computing k until we have at least n terms caching results */
    while (collected_len < (size_t)n) {
        ++last_checked;                                /* next k */
        seq_t sigma = sigma_of(last_checked);           /* sum of divisors of k */
        seq_t val   = sigma - (seq_t)last_checked - (seq_t)2;
        if (is_prime_big(val)) {
            ensure_collected();
            collected[collected_len++] = (seq_t)last_checked;
        }
    }
    return collected[n - 1];
}
