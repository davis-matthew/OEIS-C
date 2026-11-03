/*
 * A387981  Positive integers m such that 2*m is not the sum of two
 *          horizontal, vertical or diagonal neighboring primes in a square
 *          spiral with odd primes.
 * https://oeis.org/A387981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth positive integer m such that * 2*m is NOT equal
 *            to the sum of two adjacent odd primes * (horizontal, vertical or
 *            diagonal) in the square spiral * obtained by placing successive
 *            odd primes in the usual * clockwise spiral order. * This
 *            implementation constructs the spiral for all od...
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Validated: partial, pass, 60 terms through n = 60 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Build the spiral coordinates for all odd primes up to limit.
 * `grid` is a 2D array flattened; offset centers the spiral.
 */
static void build_prime_spiral(const unsigned char *is_prime,
                               size_t prime_limit,
                               int *grid,
                               int offset,
                               int grid_size,
                               size_t *prime_cnt)
{
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    int x = 0, y = 0;
    int dir = 0;
    int step_len = 1;
    int step_done = 0;
    int changes = 0;
    size_t written = 0;

    /* first prime (3) at (0,0) if present */
    if (3 <= prime_limit && is_prime[3])
    {
        int idx = (x + offset) + (y + offset) * grid_size;
        grid[idx] = 3;
        ++written;
    }

    while (written < prime_cnt[0])
    {
        for (int s = 0; s < step_len; ++s)
        {
            x += dx[dir];
            y += dy[dir];
            if (9 > prime_limit) break; /* safety, unlikely */

            /* find next odd prime */
            if (x == 0 && y == 0) continue; /* already placed 3 */

            /* look for the next odd prime in order.
             * We advance a static counter to locate it. */
            /* This logic is handled after the while loop. */
        }
        /* move to next direction */
        dir = (dir + 1) & 3;
        ++changes;
        if (changes % 2 == 0)
            ++step_len;
    }
}

/* Construct the spiral array with all odd primes.
 * Each odd prime is placed in the next cell of the normal
 * Ulam spiral (clockwise, starting east).
 */
static void build_spiral_grid(const uint32_t *primes,
                              size_t prime_cnt,
                              int *grid,
                              int offset,
                              int grid_size)
{
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    int x = 0, y = 0;
    int dir = 0;
    int step_len = 1;
    int changes = 0;
    size_t idxp = 0;

    /* place first prime (3) at center */
    int idx = (x + offset) + (y + offset) * grid_size;
    grid[idx] = (int)primes[idxp++];
    while (idxp < prime_cnt)
    {
        for (int s = 0; s < step_len && idxp < prime_cnt; ++s)
        {
            x += dx[dir];
            y += dy[dir];
            idx = (x + offset) + (y + offset) * grid_size;
            grid[idx] = (int)primes[idxp++];
        }
        dir = (dir + 1) & 3;
        ++changes;
        if (changes % 2 == 0)
            ++step_len;
    }
}

/* Generate all odd primes up to prime_limit and store them into
 * an array `primes_out` of length `prime_cnt`.
 */
static void list_odd_primes(const unsigned char *is_prime,
                            size_t prime_limit,
                            uint32_t **primes_out,
                            size_t *prime_cnt)
{
    size_t cnt = 0;
    for (size_t i = 3; i <= prime_limit; i += 2)
        if (is_prime[i])
            ++cnt;
    *prime_cnt = cnt;
    *primes_out = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (size_t i = 3; i <= prime_limit; i += 2)
        if (is_prime[i])
            (*primes_out)[idx++] = (uint32_t)i;
}

/* Helper: linear sieve for primes up to MAX_PRIME */
static void sieve_bool(unsigned char *is_prime, size_t n)
{
    size_t i, j;
    memset(is_prime, 1, n + 1);
    is_prime[0] = is_prime[1] = 0;
    for (i = 2; i * i <= n; ++i)
        if (is_prime[i])
            for (j = i * i; j <= n; j += i)
                is_prime[j] = 0;
}

/* Size limits for precomputation */
#define MAX_M          40000              /* maximum m we are ready to answer */
#define MAX_PRIME      (2 * MAX_M)        /* primes up to this value are needed */
#define GRID_GROWTH    2
#define INITIAL_CAP    64

/* Main function that returns the nth term. */
seq_t a(idx_t n)
{
    /* static storage for sequence terms and intermediate data */
    static seq_t *terms = NULL;
    static size_t terms_size = 0;
    static size_t terms_cap = 0;
    static bool data_ready = false;

    /* --- First call: precompute all data --- */
    if (!data_ready)
    {
        /* 1. Prime sieve up to MAX_PRIME */
        unsigned char *is_prime = (unsigned char *)malloc((MAX_PRIME + 1) * sizeof(unsigned char));
        sieve_bool(is_prime, MAX_PRIME);

        /* 2. List all odd primes */
        uint32_t *primes = NULL;
        size_t prime_cnt = 0;
        list_odd_primes(is_prime, MAX_PRIME, &primes, &prime_cnt);

        /* 3. Build coordinate grid */
        int offset = 200;  /* sufficient for prime_cnt ~ 7000 */
        int grid_size = offset * 2 + 1;
        int *grid = (int *)calloc(grid_size * grid_size, sizeof(int));
        if (!grid) { free(is_prime); free(primes); return 0; }
        build_spiral_grid(primes, prime_cnt, grid, offset, grid_size);

        /* 4. Forbidden sums up to 2*MAX_M */
        size_t forbidden_size = 2 * MAX_M + 1;
        bool *forbidden = (bool *)calloc(forbidden_size, sizeof(bool));

        /* Neighbor offsets */
        int ndx[] = {1, -1, 0, 0, 1, 1, -1, -1};
        int ndy[] = {0, 0, 1, -1, 1, -1, 1, -1};

        for (int y = 0; y < grid_size; ++y)
            for (int x = 0; x < grid_size; ++x)
            {
                int p = grid[x + y * grid_size];
                if (!p) continue;
                for (int k = 0; k < 8; ++k)
                {
                    int nx = x + ndx[k];
                    int ny = y + ndy[k];
                    if (nx < 0 || ny < 0 || nx >= grid_size || ny >= grid_size) continue;
                    int q = grid[nx + ny * grid_size];
                    if (!q) continue;
                    size_t s = (size_t)p + (size_t)q;
                    if (s < forbidden_size) forbidden[s] = true;
                }
            }

        /* 5. Build sequence up to needed size (up to 20000 terms) */
        const size_t MAX_TERMS = 20000;
        terms = mem_new(seq_t, MAX_TERMS);
        terms_cap = MAX_TERMS;
        terms_size = 0;

        seq_t m = 1;
        while (terms_size < MAX_TERMS && (size_t)(m * 2) < forbidden_size)
        {
            size_t s = (size_t)(m * 2) ;
            if (!forbidden[s])
            {
                terms[terms_size++] = m;
            }
            ++m;
        }

        /* Free temporary data */
        free(is_prime);
        free(primes);
        free(grid);
        free(forbidden);
        data_ready = true;
    }

    /* --- Return requested term --- */
    if (n == 0 || (size_t)n > terms_size)
        return 0;  /* undefined for n beyond precomputed range */
    return terms[n - 1];
}
