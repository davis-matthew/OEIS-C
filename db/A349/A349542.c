/*
 * A349542  Number of partitions of n into distinct unitary divisors of n.
 * https://oeis.org/A349542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct unitary divisors
 *            of n
 * Range:     u8 n <= 2309, u16 n <= 2309, u32 n <= 9498, u64 n <= 9498,
 *            u128 n <= 9498, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------- Helper functions for prime generation ------------------- */
static int  primes[1225];
static int  primes_cnt = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready) return;
    int limit = 10000;                     /* sufficient for all n <=  10000   */
    static unsigned char is_comp[10001] = {0};
    for (int i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            primes[primes_cnt++] = i;
            if ((long long)i * i <= limit)
                for (int j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
        }
    }
    primes_ready = true;
}

/* --------------- Main sequence function --------------------------------- */
seq_t a(idx_t n)
{
    /* Handle offset n = 0 */
    if (n == 0) return 1;

    init_primes();

    /* Factorise n into distinct prime power factors (unitary divisor components) */
    uint64_t n_orig = (uint64_t)n;
    uint64_t m = n_orig;
    uint64_t prime_powers[6]; /* n <= 10000 → at most 5 distinct primes */
    int k = 0;

    for (int pi = 0; pi < primes_cnt && primes[pi] * (uint64_t)primes[pi] <= m && k < 6; ++pi) {
        if (m % primes[pi] == 0) {
            uint64_t p = primes[pi];
            uint64_t pow = 1;
            while (m % p == 0) {
                m /= p;
                pow *= p;
            }
            prime_powers[k++] = pow;
        }
    }
    if (m > 1 && k < 6) {
        prime_powers[k++] = m;   /* remaining prime factor */
    }

    /* Generate all unitary divisors: products of subsets of prime_powers */
    size_t unit_count = (size_t)1 << k;          /* 2^k */
    uint64_t *units = (uint64_t*)malloc(unit_count * sizeof(uint64_t));
    if (!units) return 0; /* allocation failure, unlikely */

    for (size_t mask = 0; mask < unit_count; ++mask) {
        uint64_t prod = 1;
        for (int i = 0; i < k; ++i) {
            if (mask & ((size_t)1 << i))
                prod *= prime_powers[i];
        }
        units[mask] = prod;
    }

    /* Subset‑sum DP: count subsets of units summing to n */
    seq_t *dp = (seq_t*)calloc((size_t)n_orig + 1, sizeof(seq_t));
    if (!dp) { free(units); return 0; }  /* allocation failure */

    dp[0] = 1;   /* empty subset */
    for (size_t ui = 0; ui < unit_count; ++ui) {
        uint64_t val64 = units[ui];
        if (val64 == 0) continue;  /* never occurs for n > 0 */
        idx_t u = (idx_t)val64;
        for (idx_t s = n_orig; s >= u; --s)
            dp[s] += dp[s - u];
    }

    seq_t result = dp[n_orig];

    free(dp);
    free(units);
    return result;
}
