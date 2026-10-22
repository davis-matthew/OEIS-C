/*
 * A399093  The maximum exponent in the prime factorization of the
 *          exponentially odd numbers k such that k+1 is also exponentially
 *          odd.
 * https://oeis.org/A399093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A399093: maximal exponent in the prime factorisation of the nth
   exponentially odd number k with the property that k+1 is also exponentially odd.
   The function scans k from 1 upward, testing the exponential‑odd property for
   k and k+1 using a precomputed smallest prime factor array, counting until the
   desired index n is reached.  It then returns the maximum exponent found in k.
   The algorithm runs in O(m log log m) time for the first m numbers, easily
   fast enough for n up to about 10 000. */
#include <stdlib.h>
#include <stdbool.h>

static idx_t *spf = NULL;          /* smallest prime factor array */
static idx_t spf_cap = 0;          /* allocated size of spf */

/* Recalculate SPF table up to 'limit' (inclusive). */
static void recompute_spf(idx_t limit)
{
    free(spf);
    spf = (idx_t*)calloc((size_t)limit + 1, sizeof(idx_t));
    if (!spf) return;   /* allocation failure – will cause later checks to fail */
    spf_cap = limit;
    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {            /* i is prime */
            spf[i] = i;
            if (i > limit / i) continue;
            for (idx_t j = i * i; j <= limit; j += i) {
                if (spf[j] == 0) spf[j] = i;
            }
        }
    }
}

/* Test whether 'm' is exponentially odd.
   If true, '*maxexp' receives the maximum exponent in the factorisation.
   Assumes spf is filled up to at least m. */
static bool is_exponentially_odd(idx_t m, seq_t *maxexp)
{
    if (m == 1) {
        *maxexp = 0;
        return true;
    }
    seq_t mx = 0;
    while (m > 1) {
        idx_t p = spf[m];
        if (p == 0) p = m;            /* should not happen if spf is large enough */
        idx_t e = 0;
        while (m % p == 0) {
            m /= p;
            ++e;
        }
        if ((e & 1) == 0) return false;
        if (e > mx) mx = e;
    }
    *maxexp = mx;
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* not used, but safe */
    /* Initial SPF size estimation */
    idx_t limit = n * 3ULL + 200ULL;
    if (limit < 100ULL) limit = 100ULL;
    if (spf == NULL || spf_cap < limit) recompute_spf(limit);

    idx_t k = 1;
    idx_t count = 0;
    seq_t result = 0;

    while (count < n) {
        /* Ensure SPF table covers k+1 */
        if (k + 1 > spf_cap) {
            idx_t newlimit = spf_cap * 2ULL + 100ULL;
            recompute_spf(newlimit);
        }
        seq_t expk = 0, expk1 = 0;
        if (is_exponentially_odd(k, &expk) && is_exponentially_odd(k + 1, &expk1)) {
            ++count;
            if (count == n) {
                result = expk;      /* maximal exponent in k */
                break;
            }
        }
        ++k;
    }
    return result;
}
