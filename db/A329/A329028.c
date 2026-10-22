/*
 * A329028  The least missing nonzero digit in the primorial base expansion of
 *          n.
 * https://oeis.org/A329028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 573, u16 n <= 7111, u32 n <= 7111, u64 n <= 7111,
 *            u128 n <= 7111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A329028: least missing nonzero digit in primorial base expansion of n */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* First 16 primes – enough for all tests up to n ≈ 10 000.  */
static const uint64_t _first_primes[16] = {
    2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL,
    23ULL, 29ULL, 31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL
};

/*
 * Compute the least non‑zero digit that does NOT appear in the
 * primorial base representation of n.
 */
seq_t a(idx_t n)
{
    /* In an expansion the i-th weight is the product of the first i primes:
     *   w0 = 1,
     *   w1 = 2,
     *   w2 = 2·3 = 6,
     *   w3 = 2·3·5 = 30, …  */
    seq_t weights[17];
    size_t wcnt = 1;               /* weights[0] = 1 is always present */
    weights[0] = (seq_t)1;

    seq_t rem = (seq_t)n;

    /* Build weights up to the largest not exceeding n. */
    for (size_t i = 0; i < 16; ++i) {
        seq_t next = weights[wcnt - 1] * (seq_t)_first_primes[i];
        if (next > rem) {
            break;
        }
        weights[wcnt++] = next;
    }

    /* Mark which non‑zero digits actually appear. */
    bool seen[128];
    memset(seen, 0, sizeof(seen));

    for (size_t pos = wcnt; pos-- > 0; ) {
        seq_t w = weights[pos];
        seq_t q = rem / w;
        rem = rem % w;

        if (q != 0) {
            idx_t d = (idx_t)q;   /* q will fit in idx_t because primes<128 */
            if (d < sizeof(seen))
                seen[d] = true;
        }
    }

    /* Return the smallest missing non‑zero digit. */
    for (idx_t d = 1; d < (idx_t)sizeof(seen); ++d) {
        if (!seen[d]) {
            return (seq_t)d;
        }
    }

    /* Theoretically unreachable for the ranges used. */
    return (seq_t)127;
}
