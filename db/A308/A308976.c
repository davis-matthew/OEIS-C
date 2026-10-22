/*
 * A308976  Sum of the sixth largest parts of the partitions of n into 7
 *          primes.
 * https://oeis.org/A308976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Sum of the sixth largest parts of the partitions of n into 7
 *            primes
 * Range:     u8 n <= 52, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve (shared between calls) ---------- */
static uint32_t *prime_list = NULL;
static size_t   prime_cnt  = 0;
static idx_t    prime_limit = 0;

static void build_primes_upto(idx_t limit)
{
    if (limit <= prime_limit) return;

    /* free old list if present */
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
        prime_cnt  = 0;
        prime_limit = 0;
    }

    if (limit < 2) return;

    size_t size = (size_t)(limit + 1);
    bool *sieve = (bool *)calloc(size, sizeof(bool));
    if (!sieve) abort();
    for (idx_t i = 2; i <= limit; ++i) sieve[i] = true;

    for (idx_t i = 2; i * i <= limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= limit; j += i) sieve[j] = false;
        }
    }

    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) ++cnt;

    prime_list = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    if (!prime_list) abort();

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i) if (sieve[i]) prime_list[idx++] = (uint32_t)i;

    prime_cnt   = cnt;
    prime_limit = limit;
    free(sieve);
}

/* ---------- Recursive enumeration of 7‑prime multisets ---------- */
static void dfs_sum(idx_t target,
                    idx_t depth,
                    int      last_idx,
                    idx_t    sum,
                    uint32_t p2,
                    seq_t   *acc)
{
    if (depth == 7) {                 /* 7 primes chosen */
        if (sum == target) *acc += (seq_t)p2;
        return;
    }

    const size_t rem = 7 - depth;
    for (int i = last_idx; i < (int)prime_cnt; ++i) {
        uint32_t p = prime_list[i];
        idx_t new_sum = sum + p;
        if (new_sum > target) break;   /* further primes are larger */

        size_t new_rem = rem - 1;
        if (new_rem > 0 && new_sum + new_rem * (idx_t)p > target)
            break;                     /* impossible to reach target */

        uint32_t next_p2 = p2;
        if (depth == 1) next_p2 = p;   /* second prime chosen */

        dfs_sum(target, depth + 1, i, new_sum, next_p2, acc);
    }
}

/* ---------- Public interface ---------- */
seq_t a(idx_t n)
{
    if (n < 14) return (seq_t)0;       /* no partition into 7 primes */

    build_primes_upto(n);              /* primes up to n are sufficient */

    seq_t acc = 0;                     /* accumulator for the answer */
    dfs_sum(n, 0, 0, 0, 0, &acc);
    return acc;
}
