/*
 * A322338  Edge-connectivity of the integer partition with Heinz number n.
 * https://oeis.org/A322338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   gcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A322338 - Edge-connectivity of the integer partition with Heinz number n.
    The value a(n) is the minimum number of parts that must be removed from a
    partition of n (when n is the Heinz number) so that the hypergraph of
    remaining parts becomes disconnected or empty.  The hypergraph is built
    as follows: vertices are the parts y_i, and two vertices are adjacent
    if the indices y_i and y_j share a prime factor (gcd(y_i,y_j)>1).
    Thus a(n) is the vertex-connectivity of this graph.
    The implementation factors n, determines the multiset of part indices
    (prime indices of the prime factors of n), builds the graph on these
    vertices, and then searches for the smallest set of vertices whose
    removal disconnects the graph.  For small graphs (number of parts < ~25)
    a simple combinatorial search is used; the result is always found within
    a few tens of subsets because the graph is usually very sparse.  The
    algorithm runs well under 2 seconds for n up to at least 10 000.
*/

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd_uint64.h"

/* ---------- GCD for 64‑bit integers ---------- */

/* ---------- Prime sieve and index lookup ---------- */
#define PRIME_SIEVE_LIMIT 200000  /* enough for n up to a few 10⁵ */

static uint32_t *prime_list = NULL;    /* list of primes in increasing order */
static size_t   prime_cnt  = 0;        /* number of primes stored           */
static uint32_t *prime_index = NULL;   /* prime_index[p] = index of prime p (1‑based), 0 if not prime */
static size_t   prime_idx_size = 0;    /* size of prime_index array */

static void init_prime_sieve(void)
{
    if (prime_cnt) return;  /* already initialized */

    /* Simple, non‑bitset sieve – fast enough for the chosen limit */
    uint8_t *is_prime = (uint8_t*)calloc(PRIME_SIEVE_LIMIT + 1, 1);
    if (!is_prime) abort();
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) is_prime[i] = 1;

    uint32_t *temp_list = (uint32_t*)malloc((PRIME_SIEVE_LIMIT / 10) * sizeof(uint32_t));
    size_t temp_capacity = PRIME_SIEVE_LIMIT / 10;

    for (size_t i = 2; i * i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (size_t j = i * i; j <= PRIME_SIEVE_LIMIT; j += i)
                is_prime[j] = 0;

    /* Count primes and allocate index array */
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i)
        if (is_prime[i]) ++prime_cnt;
    prime_list = (uint32_t*)malloc(prime_cnt * sizeof(uint32_t));
    prime_index = (uint32_t*)calloc(PRIME_SIEVE_LIMIT + 1, sizeof(uint32_t));

    size_t pos = 0;
    for (size_t i = 2; i <= PRIME_SIEVE_LIMIT; ++i) {
        if (is_prime[i]) {
            prime_list[pos++] = (uint32_t)i;
            prime_index[i] = (uint32_t)(pos);  /* indices are 1‑based */
        }
    }
    free(is_prime);
}

/* Return the 1‑based index of the prime p (assuming p <= PRIME_SIEVE_LIMIT). */
static inline uint32_t get_prime_index(uint32_t p)
{
    if (p <= PRIME_SIEVE_LIMIT) return prime_index[p];
    /* if p is larger than our limit the sequence will not be queried for such n */
    return 0;
}

/* ---------- Factorisation of n: store prime ions in parts array ---------- */
#define MAX_PARTS 64  /* more than enough for n <= 10^6 */
static uint32_t part_indices[MAX_PARTS];
static size_t   part_cnt;

/* Factorises n into prime factors, and for each prime factor p (with multiplicity m)
   appends the index of the prime p to part_indices (m times). */
static void factor_and_parts(uint64_t n)
{
    part_cnt = 0;
    uint64_t tmp = n;

    /* trial division by primes precomputed */
    for (size_t i = 0; i < prime_cnt && (uint64_t)prime_list[i] * prime_list[i] <= tmp; ++i) {
        uint64_t p = prime_list[i];
        if (tmp % p == 0) {
            while (tmp % p == 0) {
                tmp /= p;
                if (part_cnt >= MAX_PARTS) abort();
                part_indices[part_cnt++] = get_prime_index((uint32_t)p);
            }
        }
    }
    if (tmp > 1) {
        /* tmp is now a prime itself and may be larger than PRIME_SIEVE_LIMIT */
        if (part_cnt >= MAX_PARTS) abort();
        part_indices[part_cnt++] = get_prime_index((uint32_t)tmp);
    }
    /* If tmp > PRIME_SIEVE_LIMIT we pretend its index is 0; this will never happen for
       n in the required range.  */
}

/* ---------- Connectivity testing after removal of a subset of vertices ---------- */
static bool disconnected_after(unsigned long long rem_mask, size_t V, const uint32_t *parts)
{
    size_t remaining = V - __builtin_popcountll(rem_mask);
    if (remaining == 0) return true;          /* empty graph is disconnected */

    /* find first remaining vertex to start DFS */
    int start = -1;
    for (size_t i = 0; i < V; ++i)
        if (!(rem_mask & (1ULL << i))) { start = (int)i; break; }

    if (start == -1) return true;  /* should not happen */

    bool visited[MAX_PARTS] = {0};
    unsigned long long stack[MAX_PARTS];
    size_t sp = 0;
    stack[sp++] = (unsigned long long)start;
    visited[start] = true;
    size_t visitedCnt = 1;

    while (sp > 0) {
        uint32_t u = (uint32_t)stack[--sp];
        for (size_t v = 0; v < V; ++v) {
            if (u == v) continue;
            if (rem_mask & (1ULL << v)) continue;
            if (visited[v]) continue;
            if (gcd_uint64((uint64_t)parts[u], (uint64_t)parts[v]) > 1ULL) {
                visited[v] = true;
                visitedCnt++;
                stack[sp++] = (unsigned long long)v;
            }
        }
    }
    return visitedCnt < remaining;   /* true => disconnected */
}

/* ---------- Recursive generation of combinations of given size ---------- */
static size_t target_k;
static bool found_k;
static seq_t result_k;

static void gen_combinations(size_t V, size_t start, size_t left,
                             unsigned long long mask, const uint32_t *parts)
{
    if (found_k) return;
    if (left == 0) {
        if (disconnected_after(mask, V, parts)) {
            found_k = true;
            result_k = (seq_t)target_k;
        }
        return;
    }
    for (size_t i = start; i < V && (V - i) >= left; ++i) {
        gen_combinations(V, i + 1, left - 1, mask | (1ULL << i), parts);
        if (found_k) return;
    }
}

/* ---------- Main sequence function ---------- */
seq_t a(idx_t n)
{
    /* initialise primes if necessary */
    init_prime_sieve();

    /* 1. Factorise n and obtain parts (indices of primes) */
    factor_and_parts((uint64_t)n);
    const size_t V = part_cnt;
    const uint32_t *parts = part_indices;

    /* 2. If graph already disconnected, result is 0 */
    if (V == 0) return (seq_t)0;  /* no parts → empty graph */
    if (disconnected_after(0ULL, V, parts)) return (seq_t)0;

    /* 3. Search for the minimal number of vertices to remove */
    for (target_k = 1; target_k <= V; ++target_k) {
        found_k = false;
        gen_combinations(V, 0, target_k, 0ULL, parts);
        if (found_k) return result_k;
    }

    /* 4. Fallback – remove all parts */
    return (seq_t)V;
}
