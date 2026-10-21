/*
 * A293661  Base-8 circular primes that are not base-8 repunits.
 * https://oeis.org/A293661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth base‑8 circular prime (prime in base 10) that *
 *            has only odd octal digits and is not an octal repunit. Computed
 *            by * enumerating all even‑length octal numbers with odd digits
 *            (up to length * 8, where the search terminates because no
 *            further examples exist) and * testing each rotati...
 * Range:     u8 n <= 8, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* comparison function for qsort over seq_t */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *a = (const seq_t *)p1;
    const seq_t *b = (const seq_t *)p2;
    if (*a < *b) return -1;
    if (*a > *b) return  1;
    return 0;
}

/* deterministic trial division prime test, safe for values < 2^32 */
static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }
    return true;
}

/* Global cache for sequence values (1‑based indexing). */
static seq_t  *cache = NULL;     /* array of seq_t values */
static size_t  clen  = 0;        /* number of stored values */
static size_t  ccap  = 0;        /* capacity of the array */
static bool    built = false;    /* has the list been built? */

/* enlarge the cache if needed */
static void ensure_cache_size(size_t needed)
{
    if (needed <= ccap) return;
    size_t newcap = ccap ? ccap * 2 : 16;
    while (newcap < needed) newcap *= 2;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    ccap = newcap;
}

/* build all known terms (up to length 8, which is the conjectured limit) */
static void build_sequence(void)
{
    if (built) return;
    built = true;

    /* precompute 8^(k) up to k=7 */
    uint64_t pow8[9];
    pow8[0] = 1;
    for (int i = 1; i < 9; ++i) pow8[i] = pow8[i-1] * 8ULL;

    /* generate candidates for each even octal length 2..8 */
    for (int L = 2; L <= 8; L += 2) {
        /* number of masks: 4^L */
        size_t total = 1;
        for (int i = 0; i < L; ++i) total *= 4U;
        if (total == 0) continue; /* overflow guard */

        for (size_t mask = 1; mask < total; ++mask) { /* skip mask 0 (repunit) */
            uint64_t digits[8];
            size_t tmp = mask;
            for (int pos = 0; pos < L; ++pos) {
                uint64_t d = tmp & 3ULL; /* tmp % 4 */
                tmp >>= 2;               /* tmp /= 4 */
                digits[pos] = 1ULL + 2ULL * d; /* odd digit 1,3,5,7 */
            }

            /* construct the base‑10 value from the octal digits */
            uint64_t value = 0;
            for (int pos = L - 1; pos >= 0; --pos) {
                value = value * 8ULL + digits[pos];
            }

            /* test all rotations for primality */
            uint64_t cur = value;
            bool ok = true;
            for (int r = 0; r < L; ++r) {
                if (!is_prime_u64(cur)) { ok = false; break; }
                uint64_t high = cur / pow8[L-1];
                uint64_t low  = cur % pow8[L-1];
                cur = low * 8ULL + high;
            }
            if (!ok) continue;

            /* store the value */
            ensure_cache_size(clen + 1);
            cache[clen++] = (seq_t)value;
        }
    }

    /* sort the cache into ascending order */
    if (clen > 1) qsort(cache, clen, sizeof(seq_t), cmp_seq_t);
}

/* sequence function required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but harmless */
    build_sequence();
    if ((size_t)n <= clen) return cache[(size_t)n - 1];
    /* if n exceeds the known length, return 0 (sequence is finite) */
    return 0;
}
