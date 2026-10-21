/*
 * A248147  Table read by rows: n-th row contains all consecutive subsets of
 *          the first n primes in their natural order.
 * https://oeis.org/A248147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 8855, u16 n <= 8855, u32 n <= 8855, u64 n <= 8855,
 *            u128 n <= 8855, big any n
 * Helpers:   binom_cumulative, prefix_within_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A248147 – consecutive‑subset prime enumeration */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/binom_cumulative.h"
#include "../utils/prefix_within_row.h"

/* Global storage for the first primes.  The array is grown only
 * when more primes are needed; the value of prime_count is the
 * number of primes currently stored. */
static uint64_t *primes = NULL;
static size_t  prime_count = 0;
static size_t  prime_cap   = 0;
static uint64_t next_candidate = 2;

/* Ensure that at least `needed` primes are available in the global
 * array.  The algorithm is a simple incremental sieve: each
 * candidate is tested against the primes already stored. */
static void ensure_primes(idx_t needed)
{
    if (prime_count >= needed) return;

    /* Grow the array if necessary. */
    if (prime_cap < needed)
    {
        size_t new_cap = prime_cap ? prime_cap * 2 : 16;
        while (new_cap < needed) new_cap *= 2;
        uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
        if (!tmp && primes && prime_cap > 0) tmp = primes; /* allocation failure → keep old pointer */
        primes = tmp;
        prime_cap = new_cap;
    }

    /* Add new primes until we have 'needed' of them. */
    size_t count = prime_count;
    while (count < needed)
    {
        uint64_t cand = next_candidate;
        bool is_prime = true;
        for (size_t i = 0; i < count; ++i)
        {
            uint64_t p = primes[i];
            if ((unsigned __int128)p * p > cand) break;
            if (cand % p == 0)
            {
                is_prime = false;
                break;
            }
        }
        if (is_prime)
        {
            primes[count] = cand;
            ++count;
        }
        /* Advance to the next odd candidate (skip even). */
        next_candidate = (cand == 2) ? 3 : cand + 2;
    }
    prime_count = count;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* Find row r such that binomial(r+3,4) >= n. */
    idx_t low  = 1;
    idx_t high = 2;
    while (binom_cumulative(high) < n) high <<= 1;     /* grow until enough */

    while (low < high)
    {
        idx_t mid = (low + high) >> 1;
        if (binom_cumulative(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position inside the row (0‑based). */
    idx_t prev_cum = (r > 1) ? binom_cumulative(r - 1) : 0;
    idx_t pos = n - prev_cum - 1;

    /* Determine length k of the segment that contains this term. */
    idx_t klow = 1, khigh = r;
    while (klow < khigh)
    {
        idx_t kmid = (klow + khigh) >> 1;
        if (prefix_within_row(kmid, r) > pos)
            khigh = kmid;
        else
            klow = kmid + 1;
    }
    idx_t k = klow;

    /* Offset inside length‑k block. */
    idx_t f_prev = (k > 1) ? prefix_within_row(k - 1, r) : 0;
    idx_t d = pos - f_prev;
    idx_t subset_index = d / k;     /* which consecutive segment of length k */
    idx_t offset       = d % k;     /* position inside that segment */

    /* Prime index (0‑based) among the first r primes. */
    idx_t prime_pos = subset_index + offset;   /* 0 <= prime_pos < r */

    /* Ensure we have enough primes. */
    ensure_primes(prime_pos + 1);

    return (seq_t)primes[prime_pos];
}
