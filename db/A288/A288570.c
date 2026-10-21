/*
 * A288570  Partial sums of A019565.
 * https://oeis.org/A288570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = ∑_{i=0}^{n} ∏_{k: bit_k(i)=1} prime(k+1) (partial sums of
 *            A019565)
 * Range:     u8 n <= 12, u16 n <= 61, u32 n <= 973, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Dynamic array of the first prime numbers */
static uint64_t *primes = NULL;      /* array of primes */
static size_t prime_cnt = 0;         /* number of primes stored */
static size_t prime_cap = 0;         /* allocated capacity */

/* Simple prime test using the primes already in the array */
static bool is_new_prime(uint64_t cand)
{
    if (cand < 2) return false;
    for (size_t p = 0; p < prime_cnt; ++p) {
        uint64_t pr = primes[p];
        if (pr * pr > cand) break;
        if (cand % pr == 0) return false;
    }
    return true;
}

/* Grow the primes array until it contains at least 'need' primes */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    /* enlarge capacity if necessary */
    if (prime_cap < need) {
        size_t newcap = prime_cap ? prime_cap << 1 : 8;
        if (newcap < need) newcap = need;
        uint64_t *tmp = (uint64_t *)malloc(newcap * sizeof(uint64_t));
        if (prime_cnt && primes) memcpy(tmp, primes, prime_cnt * sizeof(uint64_t));
        if (primes) free(primes);
        primes = tmp;
        prime_cap = newcap;
    }

    uint64_t cand = (prime_cnt == 0) ? 2 : primes[prime_cnt - 1] + 1;
    while (prime_cnt < need) {
        if (is_new_prime(cand)) {
            primes[prime_cnt++] = cand;
        }
        ++cand;
    }
}

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Determine how many primes are needed: the highest bit position of n plus one */
    size_t need_primes = 0;
    idx_t tmp = n;
    while (tmp) { ++need_primes; tmp >>= 1; }
    ensure_primes(need_primes);

    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t prod = 1;
        idx_t mask = i;
        size_t bit = 0;
        while (mask) {
            if (mask & 1) {
                prod = prod * (seq_t)primes[bit];
            }
            mask >>= 1;
            ++bit;
        }
        sum += prod;
    }
    return sum;
}
