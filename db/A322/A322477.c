/*
 * A322477  Base-12 deletable primes (written in base 10).
 * https://oeis.org/A322477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 910, u32 n <= 910, u64 n <= 910,
 *            u128 n <= 910, big any n
 * Helpers:   grow_seq, is_prime_big, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th base‑12 deletable prime by searching successive
 * prime numbers and testing the deletable property recursively.
 * A prime p is deletable if, in base 12, removing any single digit
 * that does not create a leading zero yields either the empty string
 * (for one‑digit primes) or another deletable prime.
 */

#include <stdbool.h>
#include <stddef.h>

#include "../utils/is_prime_big.h"
#include "../utils/seq_cmp.h"
#include "../utils/grow_seq.h"

/* Memoised array of deletable primes (ascending). */
static seq_t *dp_cache = NULL;      /* array of seq_t values */
static size_t dp_cap  = 0;          /* capacity of dp_cache   */
static idx_t  dp_count = 0;         /* number of stored primes */
/* Next integer to test as a candidate prime. */
static idx_t next_candidate = 2;

/* Ensure that dp_cache contains at least n deletable primes. */
static void
ensure_deletable_upto(idx_t n)
{
    /* If already satisfied, nothing to do. */
    if (dp_count >= n)
        return;

    /* Keep searching until we have n deletable primes. */
    while (dp_count < n) {
        seq_t cand = (seq_t)next_candidate;

        /* Test primality. */
        if (is_prime_big(cand)) {
            /* Extract base‑12 digits (least significant first). */
            uint8_t raw[64];          /* 64 digits are plenty for any 64‑bit number. */
            size_t len = 0;
            seq_t t = cand;
            while (t > 0) {
                seq_t r = t % 12;
                raw[len++] = (uint8_t)(uint64_t)r;
                t = t / 12;
            }
            if (len == 0) {          /* should never happen for a prime >= 2 */
                raw[len++] = 0;
            }

            /* Reverse to get most significant first. */
            uint8_t msb[64];
            for (size_t i = 0; i < len; ++i)
                msb[i] = raw[len - 1 - i];

            bool deletable = false;

            if (len == 1) {
                /* Single‑digit primes are deletable. */
                deletable = true;
            } else {
                /* Try deleting each digit that does not produce a leading zero. */
                for (size_t pos = 0; pos < len && !deletable; ++pos) {
                    if (pos == 0 && msb[1] == 0)
                        continue;   /* would lead with a zero */

                    /* Build the number resulting from deleting msb[pos]. */
                    seq_t q = 0;
                    for (size_t i = 0; i < len; ++i) {
                        if (i == pos)
                            continue;
                        q = q * 12 + msb[i];
                    }

                    /* Binary search dp_cache for q. */
                    size_t lo = 0, hi = dp_count;
                    while (lo < hi) {
                        size_t mid = (lo + hi) / 2;
                        int cmp = seq_cmp(&dp_cache[mid], &q);
                        if (cmp == 0) {
                            deletable = true;
                            break;
                        } else if (cmp < 0) {
                            lo = mid + 1;
                        } else {
                            hi = mid;
                        }
                    }
                }
            }

            if (deletable) {
                /* Append cand to the cache. */
                grow_seq(&dp_cache, &dp_cap, dp_count + 1);
                dp_cache[dp_count++] = cand;
            }
        }

        ++next_candidate;
    }
}

/* Return the n‑th base‑12 deletable prime (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined for offset 1; harmless */

    ensure_deletable_upto(n);
    return dp_cache[n - 1];
}
