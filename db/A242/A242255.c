/*
 * A242255  Primes-greedy summable primes, as defined at A242252.
 * https://oeis.org/A242255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   binary_search_last_le, ensure_capacity, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate primes incrementally, test each prime p with the greedy
 * summation algorithm: let r = p, repeatedly subtract the largest prime
 * strictly smaller than p that is ≤ r (after the first step r < p).  If r
 * becomes 0 the prime is added to the sequence.  The function memoizes all
 * computed sequence values and all generated primes, so repeated calls with
 * arbitrary n are fast. */
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/ensure_capacity.h"
#include "../utils/binary_search_last_le.h"

seq_t a(idx_t n)
{
    /* Static storage for primes and sequence values. */
    static seq_t *primes = NULL;
    static size_t prime_len = 0, prime_cap = 0;
    static seq_t *seq = NULL;
    static size_t seq_len = 0, seq_cap = 0;
    static seq_t last_checked = 0;

    /* Generate terms until we have at least n of them. */
    while (seq_len < n) {
        /* Find the next prime after last_checked. */
        seq_t cand = last_checked + 1;
        while (!is_prime_big(cand)) ++cand;

        /* Append prime to the primes list. */
        ensure_capacity(prime_len + 1, &prime_cap, (void **)&primes,
                        sizeof(seq_t));
        primes[prime_len++] = cand;
        last_checked = cand;

        /* Test whether cand is a primes‑greedy summable prime. */
        seq_t p = cand;
        seq_t rem = p;
        bool ok = true;
        while (rem > 0) {
            if (rem < 2) { ok = false; break; }
            seq_t limit = (rem == p) ? (p - 1) : rem;
            size_t idx = binary_search_last_le(primes, prime_len, limit);
            if (idx == (size_t)-1) { ok = false; break; }
            rem -= primes[idx];
        }
        if (ok) {
            ensure_capacity(seq_len + 1, &seq_cap, (void **)&seq,
                            sizeof(seq_t));
            seq[seq_len++] = p;
        }
    }

    return seq[n - 1];
}
