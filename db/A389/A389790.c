/*
 * A389790  Number of ways to write 2*n as p + p' + q + q', where p and q are
 *          primes with p <= q, and r' is the first prime greater than r.
 * https://oeis.org/A389790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of unordered pairs of primes (p,q) with p <= q and p +
 *            next_prime(p) + q + next_prime(q) = 2*n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the function relies on a simple sieve each time a larger
   n is requested; it does not need any external helpers.  */

#include <stddef.h>     /* size_t */
#include <stdint.h>     /* uint64_t */
#include <stdlib.h>     /* malloc, free */

/* Cached data */
static struct {
    idx_t max_n;          /* maximum n for which cache is valid */
    size_t pi_cnt;        /* number of primes up to 2*max_n */
    uint64_t *primes;     /* prime list */
    size_t s_cnt;         /* number of p + next(p) values */
    uint64_t *s;          /* sorted list of s values */
} cache = {0, 0, NULL, 0, NULL};

/* Simple sieve up to limit, returns prime count */
static size_t sieve_primes(idx_t limit, uint64_t **out)
{
    size_t size = (size_t)(limit + 1);
    uint8_t *is_prime = (uint8_t *)malloc(size);
    if (!is_prime) return 0;          /* allocation failure – ignore for brevity */
    for (size_t i = 0; i < size; ++i) is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (!is_prime[p]) continue;
        for (idx_t mult = p * p; mult <= limit; mult += p)
            is_prime[(size_t)mult] = 0;
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    uint64_t *pr = NULL;
    if (cnt > 0) {
        pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
        if (!pr) { free(is_prime); return 0; }
        size_t idx = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (is_prime[i]) pr[idx++] = (uint64_t)i;
    }

    free(is_prime);
    *out = pr;
    return cnt;
}

/* Rebuild cache for requested n if necessary */
static void ensure_cache(idx_t n)
{
    if (n <= cache.max_n) return;   /* cache is already good */

    /* free old data */
    free(cache.primes);
    free(cache.s);

    idx_t limit = 2 * n;            /* primes up to 2*n are enough */

    size_t pr_cnt = sieve_primes(limit, &cache.primes);
    cache.pi_cnt = pr_cnt;
    cache.max_n = n;

    /* compute s = p + next(p) for each prime except the last one */
    if (pr_cnt >= 2) {
        cache.s_cnt = pr_cnt - 1;
        cache.s = (uint64_t *)malloc(cache.s_cnt * sizeof(uint64_t));
        if (cache.s) {
            for (size_t i = 0; i < cache.s_cnt; ++i)
                cache.s[i] = cache.primes[i] + cache.primes[i + 1];
        }
    } else {
        cache.s_cnt = 0;
        cache.s = NULL;
    }
}

/* Count unordered pairs of s values that sum to target */
static seq_t count_pairs(uint64_t *s, size_t m, uint64_t target)
{
    uint64_t total = 0;
    size_t left = 0;
    size_t right = (m == 0) ? 0 : m - 1;

    while (left < m && right < m && left <= right) {
        uint64_t sum = s[left] + s[right];
        if (sum < target) {
            ++left;
        } else if (sum > target) {
            if (right == 0) break;    /* avoid underflow */
            --right;
        } else { /* sum == target */
            if (s[left] == s[right]) {
                uint64_t len = (uint64_t)(right - left + 1);
                total += len * (len + 1) / 2;
                break;
            } else {
                uint64_t valL = s[left];
                size_t cntL = 1;
                while (left + cntL < right && s[left + cntL] == valL) ++cntL;
                uint64_t valR = s[right];
                size_t cntR = 1;
                while (right >= left + cntL && s[right - cntR] == valR) ++cntR;
                total += (uint64_t)cntL * (uint64_t)cntR;
                left  += cntL;
                if (right < cntR) break;
                right -= cntR;
            }
        }
    }
    return (seq_t)total;
}

/* Required function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    ensure_cache(n);
    uint64_t target = 2 * n;
    return count_pairs(cache.s, cache.s_cnt, target);
}
