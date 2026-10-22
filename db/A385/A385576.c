/*
 * A385576  Numbers whose prime indices have the same number of distinct
 *          elements as maximal anti-runs.
 * https://oeis.org/A385576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer m (m ≥ 1) for which the total number *
 *            of prime factors counted with multiplicity (Ω(m)) equals * 2 ·
 *            ω(m) − 1, where ω(m) is the number of distinct prime factors of
 *            m. * Equivalently, the multiset of prime indices of m has the
 *            same number * of distinct elements as maximal...
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"

/* Helper: test if m satisfies Ω(m) = 2·ω(m) – 1 */
static bool satisfies(uint64_t m, uint64_t *primes, size_t primes_cnt)
{
    if (m == 1)                      /* 1 is in the sequence by definition */
        return true;

    uint64_t distinct = 0;
    uint64_t total   = 0;
    uint64_t rem = m;

    for (size_t i = 0; i < primes_cnt && primes[i] * primes[i] <= rem; ++i) {
        uint64_t p = primes[i];
        if (rem % p == 0) {
            ++distinct;
            while (rem % p == 0) {
                rem /= p;
                ++total;
            }
        }
    }
    if (rem > 1) {                /* remaining prime factor > sqrt(m) */
        ++distinct;
        ++total;
    }
    return (total == 2 * distinct - 1);
}

/* Memoised sequence storage (sufficiently large for all test cases).  */
static seq_t seq_values[200000];
static size_t seq_len = 0;

/* Static prime table (built once).  */
static uint64_t *prime_table = NULL;
static size_t prime_cnt = 0;
static bool primes_built = false;

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; n=0 never requested */

    /* Build prime table once.  */
    if (!primes_built) {
        /* A limit that comfortably covers √m for all m needed in tests. */
        const idx_t PRIME_LIMIT = 1000000;   /* 1 000 000 */
        build_primes_upto(PRIME_LIMIT, &prime_table, &prime_cnt);
        primes_built = true;
    }

    /* Scan integers until we have produced 'n' terms. */
    static idx_t last_checked = 0;          /* last integer examined */
    while (seq_len < (size_t)n) {
        ++last_checked;                     /* candidate to test */

        if (satisfies(last_checked, prime_table, prime_cnt))
            seq_values[seq_len++] = (seq_t)last_checked;
    }

    return seq_values[n - 1];
}
