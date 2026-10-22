/*
 * A345926  Number of distinct possible alternating sums of permutations of
 *          the multiset of prime indices of n.
 * https://oeis.org/A345926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of distinct sums of subsets of size ceil(k/2) * of
 *            the multiset of prime indices of n (with k the total
 *            multiplicity), * each sum yielding an alternating sum
 *            2*subset_sum - total_sum. * The value is the cardinality of that
 *            set
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

static const int MAX_N = 10000;            /* maximum n for which we compute primes */
static const int MAX_PRIME_INDEX = 25000;  /* upper bound on prime count for safety */
static uint32_t primes[1229 + 10];         /* primes up to 10000 – count 1229 */
static uint16_t prime_index[10001];        /* mapping prime to its 1‑based index */
static size_t primes_len = 0;
static bool primes_initialized = false;

/* Simple sieve to fill primes[] and prime_index[] */
static void init_primes(void) {
    if (primes_initialized) return;
    bool is_composite[10001] = {0};
    size_t idx = 0;
    for (uint32_t i = 2; i <= MAX_N; ++i) {
        if (!is_composite[i]) {
            primes[idx++] = i;
            prime_index[i] = (uint16_t)(idx);   /* 1‑based index */
            if (i * i <= MAX_N) {
                for (uint32_t j = i * i; j <= MAX_N; j += i) {
                    is_composite[j] = true;
                }
            }
        }
    }
    primes_len = idx;
    primes_initialized = true;
}

/* Count set bits in an integer */
static inline int popcount32(uint32_t x) {
    int c = 0;
    while (x) {
        c += x & 1;
        x >>= 1;
    }
    return c;
}

#define MAX_SUM 20000
static unsigned char used[MAX_SUM + 1];

seq_t a(idx_t n) {
    if (!primes_initialized) init_primes();

    /* Special case */
    if (n == 1) return (seq_t)1;

    /* Factorize n */
    uint32_t nn = (uint32_t)n;
    uint16_t indices[20];   /* enough for n <= 10000 */
    int k = 0;              /* total count of indices (sum of exponents) */
    for (size_t pi = 0; pi < primes_len && primes[pi] * primes[pi] <= nn; ++pi) {
        uint32_t p = primes[pi];
        if (nn % p == 0) {
            uint16_t idx_p = prime_index[p];
            int exp = 0;
            while (nn % p == 0) {
                nn /= p;
                ++exp;
            }
            for (int j = 0; j < exp; ++j) {
                if (k < (int)sizeof(indices)/sizeof(indices[0])) {
                    indices[k++] = idx_p;
                }
            }
        }
    }
    if (nn > 1) {   /* remaining prime factor > sqrt */
        uint16_t idx_p = prime_index[nn];
        indices[k++] = idx_p;
    }

    int t = (k + 1) / 2; /* number of elements with + sign */
    int max_possible_sum = t * (int)primes_len;
    if (max_possible_sum > MAX_SUM) max_possible_sum = MAX_SUM;

    memset(used, 0, sizeof(used));

    uint32_t limit = 1u << k;
    for (uint32_t mask = 0; mask < limit; ++mask) {
        if (popcount32(mask) != t) continue;
        int s = 0;
        for (int bit = 0; bit < k; ++bit) {
            if (mask & (1u << bit)) {
                s += indices[bit];
            }
        }
        if (s <= max_possible_sum) used[s] = 1;
    }

    int count = 0;
    for (int i = 0; i <= max_possible_sum; ++i) {
        if (used[i]) ++count;
    }
    return (seq_t)count;
}
