/*
 * A307345  Numbers k such that every prime p <= sqrt(k) divides k*(k-1).
 * https://oeis.org/A307345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 32, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer k with the property that every prime p ≤ √k divides k·(k‑1) */
#include <stdlib.h>
#include <stdbool.h>

/* maximum known term – the sequence stops at k = 120 */
static const idx_t MAX_K = 120;

/* Small primes needed for the test; all primes ≤ √120 are 2,3,5,7,11 */
static const uint64_t PRIMES[] = {2,3,5,7,11};
static const size_t NUM_PRIMES = sizeof(PRIMES)/sizeof(PRIMES[0]);

/* integer floor of sqrt(n) – n is <= 120 so this trivial loop is fast */
static idx_t isqrt_u64(idx_t n)
{
    idx_t r = 0;
    while ((r+1)*(r+1) <= n) r++;
    return r;
}

/* true iff every prime p ≤ √k divides k·(k‑1) */
static bool good_k(idx_t k)
{
    idx_t sqrtk = isqrt_u64(k);
    for (size_t i = 0; i < NUM_PRIMES; ++i) {
        uint64_t p = PRIMES[i];
        if (p > sqrtk) break;
        if ((k % p != 0) && ((k-1) % p != 0)) return false;
    }
    return true;
}

/* compute a(n) – static cache is used so subsequent calls are cheap */
seq_t a(idx_t n)
{
    /* a simple static cache of values found so far */
    static seq_t *cache = NULL;
    static size_t sz = 0;
    static const size_t CAP = 200;   /* enough for all known terms */

    if (!cache) {
        cache = (seq_t*)malloc(CAP * sizeof(seq_t));
        sz = 0;
    }

    /* start searching from the last found value + 1 (or 1) */
    idx_t k = (sz == 0) ? 1 : (idx_t)cache[sz-1] + 1;

    while (sz < n && k <= MAX_K) {
        if (good_k(k)) {
            if (sz < CAP) cache[sz++] = (seq_t)k;
        }
        ++k;
    }

    /* if n exceeds the number of known terms, return 0 */
    if (n > sz) return 0;
    return cache[n-1];
}
