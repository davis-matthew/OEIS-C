/*
 * A371451  Number of connected components of the binary indices of the prime
 *          indices of n.
 * https://oeis.org/A371451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of connected components of the binary indices of
 *            the prime indices of n. For each prime divisor p of n, let m =
 *            index of prime p (1‑based). Its binary indices are the positions
 *            (1‑based) of 1 bits in the binary representation of m. All
 *            positions belonging to the same m are united. T...
 * Range:     u8 n <= 9772, u16 n <= 9772, u32 n <= 9772, u64 n <= 9772,
 *            u128 n <= 9772, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define UF_MAX 128  /* enough for bit positions of prime indices up to 10000 */

static bool initialized = false;
static uint32_t *prime_idx = NULL;  /* prime_idx[p] = index of prime p (1‑based), 0 if not prime */
static uint32_t *primes     = NULL; /* list of primes up to PRIME_LIMIT */
static size_t   prime_cnt   = 0;    /* number of primes */

/* ---------------------------------------------------------------------- */
/*  Initialise primes (up to a fixed limit that covers all primes <= 10000) */
/* ---------------------------------------------------------------------- */
static void init_primes(void)
{
    if (initialized) return;
    initialized = true;

    /* choose a limit sufficiently larger than 10000 to include all prime indices */
    const uint32_t PRIME_LIMIT = 2000000;   /* more than enough for primes <= 10000 */

    prime_idx = (uint32_t *)calloc(PRIME_LIMIT + 1, sizeof(uint32_t));
    if (!prime_idx) exit(1);                       /* memory error */

    bool *is_prime = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!is_prime) exit(1);

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;

    for (uint32_t i = 2; (uint64_t)i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }

    size_t est_cnt = PRIME_LIMIT / 10 + 100;
    primes = (uint32_t *)malloc(est_cnt * sizeof(uint32_t));
    if (!primes) exit(1);

    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            prime_idx[i] = (uint32_t)(prime_cnt + 1);   /* 1‑based index */
            primes[prime_cnt++] = i;
        }
    }

    free(is_prime);
}

/* ---------------------------------------------------------------------- */
/*  Union–Find helpers                                                   */
/* ---------------------------------------------------------------------- */
static int uf_parent[UF_MAX];

static int uf_find(int x)
{
    return uf_parent[x] == x ? x : (uf_parent[x] = uf_find(uf_parent[x]));
}

static void uf_union(int a, int b)
{
    int pa = uf_find(a), pb = uf_find(b);
    if (pa != pb) uf_parent[pb] = pa;
}

/* ---------------------------------------------------------------------- */
/*  Main sequence function                                               */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(1) = 0 */

    init_primes();

    /* Factorise n into unique prime factors (multiplicity ignored for indices) */
    uint64_t m = n;
    uint64_t factors[64];
    size_t fc = 0;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * (uint64_t)p > m) break;
        if (m % p == 0) {
            factors[fc++] = p;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        factors[fc++] = m;               /* remaining prime factor */
    }

    /* Prepare Union–Find and tracking arrays */
    for (int i = 0; i < UF_MAX; ++i) {
        uf_parent[i] = i;
    }
    bool used[UF_MAX];
    bool root_used[UF_MAX];
    memset(used, 0, sizeof(used));
    memset(root_used, 0, sizeof(root_used));

    /* Process each prime factor */
    for (size_t fi = 0; fi < fc; ++fi) {
        uint32_t p = (uint32_t)factors[fi];
        uint32_t idx = 0;
        if (p <= (uint32_t)prime_cnt) idx = prime_idx[p];
        if (idx == 0) continue;          /* safety: p not found in the sieve */

        /* Gather 1‑based bit positions where idx has a 1 in its binary representation */
        int positions[64];
        size_t pc = 0;
        uint32_t tmp = idx;
        int bit = 1;
        while (tmp) {
            if (tmp & 1) {
                positions[pc++] = bit;
                used[bit] = true;           /* mark this position as used */
            }
            tmp >>= 1;
            ++bit;
        }
        if (pc == 0) continue;            /* impossible, but keep safe */

        /* Union all positions in this set */
        for (size_t k = 1; k < pc; ++k) uf_union(positions[0], positions[k]);
    }

    /* Count distinct components: unique roots among used positions */
    size_t comps = 0;
    for (int i = 1; i < UF_MAX; ++i) {
        if (used[i]) {
            int r = uf_find(i);
            if (!root_used[r]) {
                root_used[r] = true;
                ++comps;
            }
        }
    }

    return (seq_t)comps;
}
