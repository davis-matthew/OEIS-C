/*
 * A395636  a(n) = number of triples (x, y, z) such that 2*x^2 + y*z = n,
 *          where x, y, z are distinct primes satisfying y < z.
 * https://oeis.org/A395636
 *
 * Offset:    23    (a(23) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A395636: a(n) = number of triples of distinct primes (x,y,z) with y<z
 * such that 2*x^2 + y*z = n.
 * The implementation simply enumerates all possible x and factorises the
 * remaining value into y*z. No memoisation is required for n up to ~10000.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* integer square‑root: floor(sqrt(n)) */
static uint64_t isqrt_uint64(uint64_t n)
{
    uint64_t low = 0, high = n, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        if (mid == 0) { low = 1; continue; }
        if (mid > n / mid)
            high = mid - 1;
        else {
            ans = mid;
            low = mid + 1;
        }
    }
    return ans;
}

/* Build a prime table up to 'limit'.  The table is re‑used between calls. */
static uint8_t *prime_flag          = NULL;   /* is_prime[i] == 1 if i is prime */
static size_t   prime_flag_cap      = 0;      /* current size of prime_flag */
static void build_primes_upto(uint64_t limit)
{
    if (limit < 2) return;
    /* Grow the flag array if needed */
    if (prime_flag_cap <= limit) {
        uint8_t *new_flag = (uint8_t *)realloc(prime_flag,
                                                (limit + 1) * sizeof(uint8_t));
        if (!new_flag) return;          /* silently abort on OOM */
        prime_flag = new_flag;
        prime_flag_cap = limit + 1;
    }

    /* initialise all as prime */
    for (uint64_t i = 0; i <= limit; ++i) prime_flag[i] = 1;
    if (limit >= 0) prime_flag[0] = 0;
    if (limit >= 1) prime_flag[1] = 0;

    /* simple Eratosthenes sieve */
    for (uint64_t p = 2; p * p <= limit; ++p) {
        if (!prime_flag[p]) continue;
        for (uint64_t m = p * p; m <= limit; m += p)
            prime_flag[m] = 0;
    }
}

seq_t a(idx_t n)
{
    if (n < 23) return 0;          /* undefined below offset; return 0 */

    uint64_t N   = (uint64_t)n;
    uint64_t lim = N;              /* maximum prime needed for factorisation */
    build_primes_upto(lim);

    uint64_t max_x = isqrt_uint64(N / 2);
    seq_t    cnt   = 0;

    for (uint64_t x = 2; x <= max_x; ++x) {
        if (!prime_flag[x]) continue;            /* x must be prime */

        uint64_t rem = N - 2 * x * x;
        if (rem <= 1) continue;

        uint64_t max_y = isqrt_uint64(rem);
        for (uint64_t y = 2; y <= max_y; ++y) {
            if (!prime_flag[y]) continue;        /* y must be prime */
            if (x == y) continue;                /* distinctness */

            if (rem % y != 0) continue;
            uint64_t z = rem / y;
            if (y >= z) break;                   /* enforce y < z and avoid double count */

            if (z > lim) continue;               /* safety guard */
            if (!prime_flag[z]) continue;        /* z must be prime */
            if (z == x) continue;                /* distinctness */

            cnt++;                               /* valid triple found */
        }
    }

    return cnt;
}
