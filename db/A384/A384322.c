/*
 * A384322  Heinz numbers of strict integer partitions with more than one
 *          possible way to choose disjoint strict partitions of each part,
 *          i.e., strict partitions that can be properly refined.
 * https://oeis.org/A384322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth Heinz number of a strict integer partition that
 *            * can be properly refined. The function constructs the sequence
 *            by * generating all square‑free numbers, testing whether their
 *            prime indices * form a strict partition that admits at least one
 *            proper refinement, * and keeping the numb...
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Constants and global storage ---------- */
#define MAX_SIEVE 200000            /* enough for N up to about 10000th term */
#define INITIAL_CAP 128

/* Smallest prime factor of numbers up to MAX_SIEVE */
static uint32_t spf[MAX_SIEVE + 1];
/* prime index (1‑based) of a prime value; 0 if not prime */
static uint32_t prime_index[MAX_SIEVE + 1];
/* prime value of a given index (1‑based) */
static uint32_t prime_of_index[MAX_SIEVE + 1];
static size_t prime_count = 0;

/* Values of the sequence */
static seq_t *values = NULL;
static size_t values_cap = 0;
static size_t values_cnt = 0;

static size_t current_limit = 1;  /* highest number examined so far */
static bool initialized = false;

/* ---------- Utility functions ---------- */
static void sieve(void)
{
    for (size_t i = 0; i <= MAX_SIEVE; ++i) {
        spf[i] = 0;
        prime_index[i] = 0;
        prime_of_index[i] = 0;
    }
    prime_count = 0;
    for (uint32_t i = 2; i <= MAX_SIEVE; ++i) {
        if (!spf[i]) {
            spf[i] = i;
            ++prime_count;
            prime_index[i] = (uint32_t)prime_count;
            prime_of_index[prime_count] = i;
            if ((uint64_t)i * i <= MAX_SIEVE) {
                for (uint32_t j = i * i; j <= MAX_SIEVE; j += i)
                    if (!spf[j]) spf[j] = i;
            }
        }
    }
}

static inline bool contains(int *arr, size_t len, int val)
{
    for (size_t i = 0; i < len; ++i)
        if (arr[i] == val) return true;
    return false;
}

/* Test whether a number p can be partitioned into distinct parts
 * drawn from 1..p‑1 that avoid all entries in arr[0..len‑1]. */
static bool can_partition(int p, int *arr, size_t len)
{
    bool dp[201] = {0};
    dp[0] = true;
    for (int x = 1; x < p; ++x) {
        /* skip used values */
        bool skip = false;
        for (size_t i = 0; i < len; ++i)
            if (arr[i] == x) { skip = true; break; }
        if (skip) continue;
        for (int s = p; s >= x; --s)
            if (dp[s - x]) dp[s] = true;
        if (dp[p]) return true;
    }
    return dp[p];
}

/* Generate numbers up to `limit` inclusive, appending those that
 * satisfy the refinement condition to the global values array. */
static void generate_up_to(size_t limit)
{
    for (size_t m = current_limit + 1; m <= limit; ++m) {
        uint32_t tmp = (uint32_t)m;
        int parts[20];
        size_t plen = 0;
        bool squarefree = true;

        while (tmp > 1) {
            uint32_t p = spf[tmp];
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            if (exp > 1) { squarefree = false; break; }
            if (p > MAX_SIEVE) { squarefree = false; break; }
            int idx = (int)prime_index[p];
            if (idx == 0) { squarefree = false; break; }
            parts[plen++] = idx;
        }
        if (!squarefree) continue;

        /* Check refinement condition */
        bool ok = false;
        for (size_t i = 0; i < plen && !ok; ++i) {
            int part = parts[i];
            if (part < 3) continue;           /* part 1 or 2 cannot be refined */

            if (part == 3) {
                if (!contains(parts, plen, 1) &&
                    !contains(parts, plen, 2))
                    ok = true;
            } else {
                if (!contains(parts, plen, 1) &&
                    !contains(parts, plen, part - 1))
                    ok = true;
            }

            /* If simple test fails, try general partition */
            if (!ok && can_partition(part, parts, plen))
                ok = true;
        }

        if (!ok) continue;

        /* Append m to the list */
        if (values_cnt == values_cap) {
            size_t new_cap = values_cap ? values_cap * 2 : INITIAL_CAP;
            seq_t *tmp = (seq_t *)realloc(values, new_cap * sizeof(seq_t));
            if (!tmp) exit(EXIT_FAILURE);
            values = tmp;
            values_cap = new_cap;
        }
        values[values_cnt++] = (seq_t)m;
    }
    current_limit = limit;
}

/* ---------- Public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but keep compiler happy */

    if (!initialized) {
        initialized = true;
        sieve();
        values = (seq_t *)malloc(INITIAL_CAP * sizeof(seq_t));
        if (!values) exit(EXIT_FAILURE);
        values_cap = INITIAL_CAP;
        values_cnt = 0;
        current_limit = 1;
    }

    /* Generate until we have at least n values */
    while (values_cnt < (size_t)n) {
        size_t new_limit = current_limit * 2 + 100;
        if (new_limit > MAX_SIEVE) new_limit = MAX_SIEVE;
        if (new_limit == current_limit) break; /* cannot extend further */
        generate_up_to(new_limit);
    }

    return values[n - 1];
}
