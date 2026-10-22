/*
 * A349410  Length of cycle reached when iterating the mapping x->
 *          n*A000005(x) on 1.
 * https://oeis.org/A349410
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of cycle reached when iterating x -> n*A000005(x)
 *            on 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation is valid for C11 and C++17.  It uses a simple
 * sieve of Eratosthenes to generate all primes up to 1 000 000,
 * which is adequate for factoring products n*y while iterating.
 * The sequence is computed on the fly for each n; no caching of
 * previous results is performed because a(n) depends only on the
 * transient cycle that emerges when starting from 1. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Prime sieve ---------- */
static const size_t PRIME_LIMIT = 1000000;   /* sqrt(10^12)   */
static uint64_t *primes_arr = NULL;          /* pointer to array of primes  */
static size_t primes_cnt = 0;                /* number of primes           */

static void
init_primes(void)
{
    if (primes_arr != NULL)
        return;   /* already built */

    size_t limit = PRIME_LIMIT;
    bool *is_comp = (bool *)calloc(limit + 1, sizeof(bool));
    if (is_comp == NULL)
        abort();

    for (size_t i = 2; i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_comp[j] = true;
        }
    }

    /* Count primes to allocate array */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            ++cnt;

    primes_arr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (primes_arr == NULL)
        abort();

    size_t index = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_comp[i])
            primes_arr[index++] = i;

    primes_cnt = cnt;
    free(is_comp);
}

/* ---------- Divisor count for 64‑bit integers ---------- */
static uint64_t
divisor_count_u64(uint64_t m)
{
    if (m == 0)
        return 0;

    init_primes();

    uint64_t result = 1;
    uint64_t temp = m;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes_arr[i];
        if (p * p > temp)
            break;
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            result *= (exp + 1);
        }
    }
    if (temp > 1)               /* remaining prime factor */
        result *= 2;

    return result;
}

/* ---------- Main sequence function ---------- */
seq_t
a(idx_t n)
{
    /* visited array: index = current y value,
     * value = step number the value first appeared (1‑based).
     * 0 means unvisited.  We grow the array when needed. */
    size_t visited_size = 128;
    uint32_t *visited = (uint32_t *)calloc(visited_size, sizeof(uint32_t));
    if (visited == NULL)
        abort();

    uint64_t y = 1;          /* current y */
    uint32_t step = 1;       /* step counter (y[1] = 1) */
    visited[1] = step;       /* mark first appearance */

    while (true) {
        /* next_y = d(n * y) */
        uint64_t prod = (uint64_t)n * y;
        uint64_t next_y = divisor_count_u64(prod);

        ++step;

        /* grow visited array if necessary */
        if (next_y >= visited_size) {
            size_t new_size = visited_size * 2;
            if (next_y + 1 > new_size)
                new_size = next_y + 1;
            uint32_t *tmp = (uint32_t *)realloc(visited,
                                                new_size * sizeof(uint32_t));
            if (tmp == NULL)
                abort();
            /* initialize new part to 0 */
            for (size_t i = visited_size; i < new_size; ++i)
                tmp[i] = 0;
            visited = tmp;
            visited_size = new_size;
        }

        if (visited[next_y] != 0) {
            uint32_t cycle_len = step - visited[next_y];
            free(visited);
            return (seq_t)cycle_len;
        }

        visited[next_y] = step;
        y = next_y;
    }
}
