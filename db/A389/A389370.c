/*
 * A389370  a(n) is the length of the longest path in the rooted tree T_n
 *          defined in A390207 and in the comments.
 * https://oeis.org/A389370
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the longest path in T_n defined in A390207. The
 *            path length is the number of edges in the longest directed path
 *            starting at the root n, where each step chooses a prime divisor
 *            p of the current node, sets d = node/p, and moves to m = node +
 *            (-1)^i * d (i = depth). A move is allowe...
 * Range:     u8 n <= 1494, u16 n <= 1494, u32 n <= 1494, u64 n <= 1494,
 *            u128 n <= 1494, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Global prime table (constructed once) */
static bool primes_built = false;
static uint64_t primes[20000];      /* enough for all primes <= 200000 */
static size_t prime_cnt = 0;

/* Build prime list up to LIMIT */
static void build_primes(void)
{
    const uint64_t LIMIT = 200000;
    bool sieve[LIMIT + 1];
    for (uint64_t i = 0; i <= LIMIT; ++i) sieve[i] = true;
    sieve[0] = sieve[1] = false;

    for (uint64_t i = 2; i * i <= LIMIT; ++i) {
        if (sieve[i]) {
            for (uint64_t j = i * i; j <= LIMIT; j += i) sieve[j] = false;
        }
    }
    prime_cnt = 0;
    for (uint64_t i = 2; i <= LIMIT; ++i) {
        if (sieve[i]) primes[prime_cnt++] = i;
    }
    primes_built = true;
}

/* Recursive depth‑first search returning the longest extension from `cur`. */
static seq_t dfs(uint64_t cur, idx_t depth, uint64_t *path, size_t path_len)
{
    /* collect distinct prime divisors of cur */
    uint64_t temp = cur;
    uint64_t divs[16];   /* at most 16 distinct primes for our range */
    size_t div_cnt = 0;

    for (size_t i = 0; i < prime_cnt && primes[i] * primes[i] <= temp; ++i) {
        uint64_t p = primes[i];
        if (temp % p == 0) {
            divs[div_cnt++] = p;
            while (temp % p == 0) temp /= p;
        }
    }
    if (temp > 1 && div_cnt < 16) divs[div_cnt++] = temp;

    seq_t best = 0;
    for (size_t i = 0; i < div_cnt; ++i) {
        uint64_t p   = divs[i];
        uint64_t d   = cur / p;
        uint64_t m   = (depth & 1) ? (cur - d) : (cur + d);

        /* check if m already used in current path */
        bool seen = false;
        for (size_t j = 0; j < path_len; ++j) {
            if (path[j] == m) {
                seen = true;
                break;
            }
        }
        if (seen) continue;

        /* extend path */
        path[path_len] = m;
        seq_t child = dfs(m, depth + 1, path, path_len + 1);
        if (child + 1 > best) best = child + 1;
    }
    return best;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 */

    if (!primes_built) build_primes();

    /* path buffer (maximum depth expected < 256) */
    static uint64_t path[256];
    path[0] = (uint64_t)n;

    return dfs((uint64_t)n, 0, path, 1);
}
