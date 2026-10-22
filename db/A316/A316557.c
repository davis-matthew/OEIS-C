/*
 * A316557  Number of distinct integer averages of subsets of the integer
 *          partition with Heinz number n.
 * https://oeis.org/A316557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of distinct integer averages of all non‑empty
 *            subsets of the multiset of indices whose corresponding primes
 *            multiply to n. For each prime factor p^e of n the index i with
 *            prime(i)=p occurs e times in the multiset. All integer averages
 *            of subsets of this multiset are collected,...
 * Range:     u8 n <= 9594, u16 n <= 9594, u32 n <= 9594, u64 n <= 9594,
 *            u128 n <= 9594, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

static int primes[1229];
static int prime_count;
static int prime_index[10001];
static bool primes_initialized = false;

/* simple sieve to fill primes[] and prime_index[] for values up to 10000 */
static void init_primes(void)
{
    bool is_prime[10001];
    memset(is_prime, true, sizeof(is_prime));
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= 10000; ++i)
        if (is_prime[i])
            for (int j = i * i; j <= 10000; j += i)
                is_prime[j] = false;

    prime_count = 0;
    for (int i = 2; i <= 10000; ++i)
        if (is_prime[i]) {
            prime_index[i] = ++prime_count;   /* indices start at 1 */
            primes[prime_count - 1] = i;
        }

    primes_initialized = true;
}

seq_t a(idx_t n)
{
    if (!primes_initialized)
        init_primes();

    if (n == 1)
        return 0;         /* no parts -> no averages */

    /* collect indices corresponding to prime factors of n */
    int idx[16];            /* k <= 13 for n <= 10000 */
    int k = 0;
    uint64_t temp = (uint64_t)n;

    for (int i = 0; i < prime_count; ++i) {
        int p = primes[i];
        if ((uint64_t)p * p > temp)
            break;
        while (temp % p == 0) {
            idx[k++] = prime_index[p];
            temp /= p;
        }
    }
    if (temp > 1)          /* remaining prime factor */
        idx[k++] = prime_index[(int)temp];

    /* visited array for distinct integer averages */
    bool visited[2000] = {0};   /* max average <= 1229 for n <= 10000 */
    seq_t count = 0;

    /* enumerate all non‑empty subsets */
    int limit = 1 << k;
    for (int mask = 1; mask < limit; ++mask) {
        seq_t sum = 0;
        int cnt = 0;
        for (int i = 0; i < k; ++i) {
            if (mask & (1 << i)) {
                sum += (seq_t)idx[i];
                ++cnt;
            }
        }
        if (sum % (seq_t)cnt == 0) {
            int avg = (int)(sum / (seq_t)cnt);
            if (!visited[avg]) {
                visited[avg] = true;
                ++count;
            }
        }
    }

    return count;
}
